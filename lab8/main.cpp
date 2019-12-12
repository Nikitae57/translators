#include "TokenListBuilder.h"

int main() {
    auto builder = TokenListBuilder();
    auto list = builder.buildTokenList("test");

    for (auto t : list) {
        int i = 0;
    }

    return 1;
}