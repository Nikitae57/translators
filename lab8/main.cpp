#include "TokenListBuilder.h"

int main() {
    auto tokenListBuilder = TokenListBuilder();
    auto list = tokenListBuilder.buildTokenList("test");

    for (auto t : list) {
        int i = 0;
    }

    return 1;
}