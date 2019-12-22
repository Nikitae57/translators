//
// Created by nikita on 12.12.2019.
//

#ifndef LAB8_ASTBUILDER_H
#define LAB8_ASTBUILDER_H

#include "TokenListBuilder.h"

enum AST_NODE_TYPE {
    WHILE, IF, ASSIGNMENT, BLOCK, ARITHMETIC_EXPR, BOOL_EXPR, ID_AST
};

struct AstNode {
    AST_NODE_TYPE type;
    vector<AstNode*> children;
    TOKEN exprToken;
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

    void makeBlockContent(const vector<TOKEN> &parentExpression, size_t index, vector<TOKEN> &returnVector);

    void makeOneLiner(const vector<TOKEN> &parentExpression, size_t index, vector<TOKEN> &returnVector);
};


#endif //LAB8_ASTBUILDER_H
