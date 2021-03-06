#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <set>


thread_local size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());

const size_t MAX_BLOCK_POWER = 15;
const size_t MAX_BLOCK_SIZE = 1 << MAX_BLOCK_POWER;
const size_t PAGE_SIZE = MAX_BLOCK_SIZE;

typedef unsigned char byte;

inline
void * writeInfo_(void * ptr, u_char info)
{
    byte * newPtr = static_cast<u_char *>(ptr);
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

typedef unsigned char byte;

class HeapStorage
{
public:
    HeapStorage(int heapNumber);
    HeapStorage(const HeapStorage &) = delete;
    void * heapAllocate(size_t bytes);
    void heapFree(byte *ptr);
    virtual ~HeapStorage();
protected:
    std::mutex mutex_;
    std::vector<byte *> blocksStorage_[MAX_BLOCK_POWER + 1];
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
        std::cerr << "HeapStorage heapAllocate: addNewBlocks has failed" << std::endl;
        return nullptr;
    }
    void * ptr = blocksStorage_[blockSizeBase].back();
    blocksStorage_[blockSizeBase].pop_back();
    return writeInfo_(ptr, blockSizeBase, heapNumber_);
}

void HeapStorage::heapFree(byte *ptr)
{
    std::unique_lock<std::mutex> lock(mutex_);
    blocksStorage_[ptr[0]].push_back(ptr);
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
    byte * newBlock = static_cast<byte *>(malloc(PAGE_SIZE));
    if (newBlock == nullptr) {
        return;
    }
    superBlocks_.emplace_back(newBlock);
    size_t blockSize = 1ul << blockSizeBase;
    size_t number = PAGE_SIZE >> blockSizeBase;
    for (size_t i = 0; i < number; ++i) {
        blocksStorage_[blockSizeBase].emplace_back(newBlock);
        newBlock += blockSize;
    }

}

class Storage
{
public:
    Storage(size_t threadNumbers=std::thread::hardware_concurrency());
    virtual ~Storage();

    void * storageAllocate(size_t bytes);

    void storageFree(void *ptr);

protected:
    HeapStorage ** heaps_;
    size_t heapNumbers_;
};

void *Storage::storageAllocate(size_t bytes)
{
    size_t needSize = bytes + 2;
    if (needSize > MAX_BLOCK_SIZE) {
        std::cout << "big piece" << std::endl;
        void * ptr = malloc(bytes + 1);
        if (ptr == nullptr)
            return nullptr;
        return writeInfo_(ptr, 0xFF);
    }

    size_t heapId = threadId % heapNumbers_;
    if (heapId >= heapNumbers_) {
        throw "heapSize";
    }
    for (size_t i = 0; i < heapNumbers_; ++i) {
        void* ans = heaps_[(heapId + i) % heapNumbers_]->heapAllocate(bytes);
        if (ans != nullptr)
            return ans;
    }
    return nullptr;
}

void Storage::storageFree(void *ptr)
{
//    std::cout << "storageFree" << std::endl;
    if (ptr == nullptr) {
        return;
    }
    u_char heapNumber;
    void * newPtr = readInfo_(ptr, heapNumber);
    if (heapNumber == 0xFF) {
        free(newPtr);
        return;
    }
    byte * mptr = ((byte*)ptr) - 2;
    heaps_[heapNumber]->heapFree(mptr);
}

Storage::Storage(size_t threadNumbers) :
    heapNumbers_(threadNumbers)
{
    heaps_ = new HeapStorage * [threadNumbers];
    for (size_t i = 0; i < threadNumbers; ++i) {
        heaps_[i] = new HeapStorage(i);
    }
}

Storage::~Storage()
{
    std::cout << "~Storage" << std::endl;
    for (size_t i = 0; i < heapNumbers_; ++i) {
        delete heaps_[i];
    }
    delete [] heaps_;
}

Storage storage;

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
    for (int j = 0; j < 20; ++j) {
        std::vector<void *> ptrs;
        std::cout << "allocating" << std::endl;
        for (int i = 0; i < 10; ++i) {
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