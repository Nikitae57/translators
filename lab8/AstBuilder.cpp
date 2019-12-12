//
// Created by nikita on 12.12.2019.
//

#include "AstBuilder.h"

bool AstBuilder::checkAndBuildExprTree(vector<TOKEN> tokens, AstNode *&root) {

}

vector<TOKEN> AstBuilder::buildBoolExprRpn(vector<TOKEN> &tokens) {
    map<SYMBOL_CLASS, int> classToPriority;
    classToPriority[OPERATION_AND] = 1;
    classToPriority[OPERATION_OR] = 1;
    classToPriority[OPERATION_EQUALS] = 1;
    classToPriority[OPERATION_NOT_EQUALS] = 1;
    classToPriority[OPEN_BRACKET] = 0;

    vector<TOKEN> result;
    stack<TOKEN> st;

    for (TOKEN currentToken : tokens) {

        switch (currentToken.type) {
            case SYMBOL_CLASS::BOOLEAN: {
                result.push_back(currentToken);
                break;
            }

            case OPEN_BRACKET: {
                st.push(currentToken);
                break;
            }

            case CLOSE_BRACKET: {
                while (st.top().type != OPEN_BRACKET) {
                    result.push_back(st.top());
                    st.pop();

                    if (st.empty()) {
                        throw out_of_range("Invalid expression");
                    }
                }
                st.pop();
                break;
            }

            default: {
                if (currentToken.type != OPERATION_AND
                    && currentToken.type != OPERATION_OR
                    && currentToken.type != OPERATION_EQUALS
                    && currentToken.type != OPERATION_NOT_EQUALS) {

                    cout << "Incorrect expression";
                    exit(-1);
                }

                while (!st.empty() && (classToPriority[currentToken.type] <= classToPriority[st.top().type])) {
                    result.push_back(st.top());
                    st.pop();
                }
                st.push(currentToken);
            }
        }
    }

    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }

    return result;
}

vector<TOKEN> AstBuilder::buildExprRpn(vector<TOKEN> &tokens) {
    map<SYMBOL_CLASS, int> classToPriority;
    classToPriority[OPERATION_ADD] = 1;
    classToPriority[OPERATION_SUB] = 1;
    classToPriority[OPERATION_MUL] = 2;
    classToPriority[OPERATION_DIV] = 2;
    classToPriority[OPEN_BRACKET] = 0;

    vector<TOKEN> result;
    stack<TOKEN> st;

    for (TOKEN currentToken : tokens) {

        switch (currentToken.type) {
            case SYMBOL_CLASS::NUMBER: {
                result.push_back(currentToken);
                break;
            }

            case OPEN_BRACKET: {
                st.push(currentToken);
                break;
            }

            case CLOSE_BRACKET: {
                while (st.top().type != OPEN_BRACKET) {
                    result.push_back(st.top());
                    st.pop();

                    if (st.empty()) {
                        throw out_of_range("Invalid expression");
                    }
                }
                st.pop();
                break;
            }

            default: {
                if (currentToken.type != OPERATION_SUB
                    && currentToken.type != OPERATION_DIV
                    && currentToken.type != OPERATION_ADD
                    && currentToken.type != OPERATION_MUL) {

                    cout << "Incorrect expression";
                    exit(-1);
                }

                while (!st.empty() && (classToPriority[currentToken.type] <= classToPriority[st.top().type])) {
                    result.push_back(st.top());
                    st.pop();
                }
                st.push(currentToken);
            }
        }
    }

    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }

    return result;
}

void AstBuilder::deleteTree(AstNode* root)
{
    if (root == nullptr) {
        return;
    }

    for (AstNode* node : root->children) {
        deleteTree(node);
    }

    delete root;
}

bool AstBuilder::checkAndBuildTree(vector<TOKEN> tokens, AstNode *&root) {

}

AstNode* AstBuilder::buildAstTree(vector<TOKEN> tokens) {
    AstNode* result = nullptr;
    if (!checkAndBuildTree(tokens, result)) {
        deleteTree(result);
        result = nullptr;
    }

    return result;
}