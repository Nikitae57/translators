#include "TokenListBuilder.h"
#include "AstBuilder.h"

int main() {
    auto tokenListBuilder = TokenListBuilder();
    auto tokenList = tokenListBuilder.buildTokenList("test");

    auto astBuilder = AstBuilder();
    AstNode* astRoot = astBuilder.buildAstTree(tokenList);
    if (astRoot == nullptr) {
        cout << "Ast built incorrectly" << endl;
    } else {
        cout << "Ast built correctly" << endl;
    }

     return 1;
}