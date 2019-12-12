//
// Created by nikita on 12.12.2019.
//

#ifndef LAB8_ASTBUILDER_H
#define LAB8_ASTBUILDER_H

#include "TokenListBuilder.h"

struct AstNode {
    TOKEN type;
    vector<AstNode*> children;
};

class AstBuilder {
private:
    bool checkAndBuildExprTree(vector<TOKEN> tokens, AstNode*& root);
    bool checkAndBuildBooleanExprTree(vector<TOKEN> tokens, AstNode*& root);

    vector<TOKEN> buildExprRpn(vector<TOKEN>& tokens);
    vector<TOKEN> buildBoolExprRpn(vector<TOKEN>& tokens);

    bool checkAndBuildTree(vector<TOKEN> tokens, AstNode*& root);
    void deleteTree(AstNode* root);

public:
    AstNode* buildAstTree(vector<TOKEN> tokens);
};


#endif //LAB8_ASTBUILDER_H
