#include "Client.h"
#include "StrategyNMoves.h"

Client client(new StrategyNMoves(5));

int main()
{
#ifdef LOCAL
    freopen("log", "w", stderr);
#endif
    client.game();
    return 0;
}
