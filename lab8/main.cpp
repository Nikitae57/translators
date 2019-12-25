#include "TokenListBuilder.h"
#include "AstBuilder.h"

int main() {
    auto tokenListBuilder = TokenListBuilder();
    auto tokenList = tokenListBuilder.buildTokenList("test");


    for (auto t : tokenList) {
        int i = 0;
    }

    auto astBuilder = AstBuilder();
    astBuilder.buildAstTree(tokenList);

    return 1;
}