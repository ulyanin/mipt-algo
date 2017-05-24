#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>


thread_local size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());

const size_t MAX_BLOCK_POWER = 15;
const size_t MAX_BLOCK_SIZE = 1 << MAX_BLOCK_POWER;
const size_t PAGE_SIZE = MAX_BLOCK_SIZE;

inline
void * writeInfo_(void * ptr, u_char info)
{
    u_char * newPtr = static_cast<u_char *>(ptr);
    newPtr[0] = info;
    return newPtr + 1;
}

inline
void * writeInfo_(void * ptr, u_char v1, u_char v2)
{
    u_char * newPtr = static_cast<u_char *>(ptr);
    newPtr[0] = v1;
    newPtr[1] = v2;
    return newPtr + 2;
}

inline
void * readInfo_(void * ptr, u_char &v)
{
    u_char * newPtr = static_cast<u_char *>(ptr);
    newPtr--;
    v = newPtr[0];
    return newPtr;
}

class HeapStorage
{
public:
    HeapStorage(int heapNumber);
    void * heapAllocate(size_t bytes);
    void heapFree(void *ptr);
    virtual ~HeapStorage();
protected:
    std::mutex mutex_;
    std::vector<void *> blocksStorage_[MAX_BLOCK_POWER + 1];
    std::vector<void *> superBlocks_;
    int heapNumber_;

    static size_t upperPowerOfTwo(size_t v);
    void addNewBlocks_(size_t blockSize);
};

HeapStorage::HeapStorage(int heapNumber) :
        heapNumber_(heapNumber)
{
}

HeapStorage::~HeapStorage()
{
    std::cout << "~HeapStorage" << std::endl;
    for (void * ptr : superBlocks_) {
        std::cout << ptr << std::endl;
        free(ptr);
    }
}

void *HeapStorage::heapAllocate(size_t bytes)
{
    std::unique_lock<std::mutex> lock(mutex_);
    size_t needSize = bytes + 2;
    size_t blockSizeBase = upperPowerOfTwo(needSize);
    if (blockSizeBase > MAX_BLOCK_POWER) {
        std::cout << blockSizeBase << " " << MAX_BLOCK_POWER << std::endl;
        std::cerr << "HeapStorage heapAllocate: too big block_base" << std::endl;
        throw std::logic_error("HeapStorage heapAllocate: too big block_base");
    }
    if (blocksStorage_[blockSizeBase].empty()) {
        addNewBlocks_(blockSizeBase);
    }
    if (blocksStorage_[blockSizeBase].empty()) {
        return nullptr;
        std::cerr << "HeapStorage heapAllocate: addNewBlocks has failed" << std::endl;
        throw std::logic_error("HeapStorage heapAllocate: addNewBlocks has failed");
    }
    u_char * ptr = (u_char *)blocksStorage_[blockSizeBase].back();
    blocksStorage_[blockSizeBase].pop_back();
//    std::cout << "write; block_size " << blockSizeBase << " " << (void *)ptr << std::endl;
    return writeInfo_(ptr,
                      static_cast<u_char>(blockSizeBase),
                      static_cast<u_char>(heapNumber_));

}

size_t HeapStorage::upperPowerOfTwo(size_t v)
{
    size_t blockPower = 3;
    while ((1ul << blockPower) < v) {
        blockPower++;
    }
    return blockPower;
}

void HeapStorage::addNewBlocks_(size_t blockSizeBase)
{
    char * newBlock = static_cast<char *>(malloc(PAGE_SIZE));
    if (newBlock == nullptr) {
        std::cout << "malloc nullptr" << std::endl;
        return;
//        throw std::bad_alloc();
    }
    superBlocks_.emplace_back(newBlock);
    size_t blockSize = 1ul << blockSizeBase;
    size_t number = PAGE_SIZE >> blockSizeBase;
    for (size_t i = 0; i < number; ++i) {
        blocksStorage_[blockSizeBase].emplace_back(newBlock);
        newBlock += blockSize;
    }

}

void HeapStorage::heapFree(void *ptr)
{
    std::unique_lock<std::mutex> lock(mutex_);
    u_char blockSizeBase, tmp;
    void * newPtr = readInfo_(ptr, tmp);
    newPtr = readInfo_(newPtr, blockSizeBase);
//    std::cout << "free: blocksize: " << (int)tmp << " " << (int)blockSizeBase << " " << ptr << std::endl;
    blocksStorage_[blockSizeBase].emplace_back(newPtr);
}

class Storage
{
public:
    Storage(int threadNumbers);
    virtual ~Storage();

    void * storageAllocate(size_t bytes);

    void storageFree(void *ptr);

protected:
    HeapStorage ** heaps_;
    int threadNumbers_;
};

void *Storage::storageAllocate(size_t bytes)
{
    size_t needSize = bytes + 2;
    if (needSize > MAX_BLOCK_SIZE) {
        std::cout << "big piece" << std::endl;
        void * ptr = malloc(bytes + 1);
        if (ptr == nullptr) {
            return nullptr;
        }
        return writeInfo_(ptr, 0xFF);
    }
    size_t heapId = threadId % threadNumbers_;
    if ((int)heapId >= threadNumbers_) {
        throw "heapSize";
    }
    for (int i = 0; i < threadNumbers_; ++i) {
        void* ans = heaps_[(heapId + i) % threadNumbers_]->heapAllocate(bytes);
        if (ans != nullptr)
            return ans;
    }
    return nullptr;
}

void Storage::storageFree(void *ptr)
{
//    std::cout << "storageFree" << std::endl;
    u_char heapId;
    void * newPtr = readInfo_(ptr, heapId);
    if (heapId == 0xFF) {
        free(newPtr);
    } else {

        if ((int)heapId >= threadNumbers_) {
            std::cout << "heapSize" << std::endl;
            throw "heapSize";
        }
        heaps_[heapId]->heapFree(ptr);
    }
}

Storage::Storage(int threadNumbers) :
        threadNumbers_(threadNumbers)
{
    heaps_ = new HeapStorage * [threadNumbers];
    for (int i = 0; i < threadNumbers; ++i) {
        heaps_[i] = new HeapStorage(i);
    }
}

Storage::~Storage()
{
    std::cout << "~Storage" << std::endl;
    for (int i = 0; i < threadNumbers_; ++i) {
        delete heaps_[i];
    }
    delete [] heaps_;
}

Storage storage(std::thread::hardware_concurrency());

extern
void* mtalloc(size_t bytes) {
//    return malloc(bytes);
//    std::cout << "storage:mtalloc" << std::endl;
    return storage.storageAllocate(bytes);
}

extern
void mtfree(void* ptr) {

//    std::cout << "storage:mtfree" << std::endl;
    storage.storageFree(ptr);
//    free(ptr);
}

void test()
{
    for (int j = 0; j < 10; ++j) {
        std::vector<void *> ptrs;
        std::cout << "allocating" << std::endl;
        for (int i = 0; i < 1; ++i) {
            std::cout << i << std::endl;
            ptrs.emplace_back(mtalloc(1 << i));
        }
        std::cout << "allocated; heapFree" << std::endl;
        for (void *ptr : ptrs) {

            mtfree(ptr);

        }
    }
    std::cout << "end" << std::endl;
}

#ifdef LOCAL

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 1; ++i) {
        threads.emplace_back(test);
    }
    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
    return 0;
}

#endif