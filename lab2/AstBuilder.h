//
// Created by nikita on 24.10.2019.
//

#ifndef LAB2_ASTBUILDER_H
#define LAB2_ASTBUILDER_H

#include "structs.h"
#include <vector>
#include <map>

struct AstNode {
    TOKEN token;
    AstNode* left = nullptr;
    AstNode* right = nullptr;
};

using namespace std;

class AstBuilder {

private:
    vector<TOKEN> buildRpn(vector<TOKEN> &expression);
    map<SYMBOL_CLASS, int> classToPriority;
    AstNode* getLastLeftFree(AstNode* root);

public:
    AstBuilder();
    AstNode* buildAstTree(vector<TOKEN> expression);
};


#endif //LAB2_ASTBUILDER_H
