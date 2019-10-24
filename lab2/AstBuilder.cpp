//
// Created by nikita on 24.10.2019.
//

#include "AstBuilder.h"
#include <stack>
#include <stdexcept>

AstBuilder::AstBuilder() {
    classToPriority[OPERATION_ADD] = 1;
    classToPriority[OPERATION_SUB] = 1;
    classToPriority[OPERATION_MUL] = 2;
    classToPriority[OPERATION_DIV] = 2;
}

vector<TOKEN> AstBuilder::buildRpn(vector<TOKEN> &expression) {

//    TOKEN currentToken;
    vector<TOKEN> result;
    stack<TOKEN> st;

    for (TOKEN currentToken : expression) {

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
                TOKEN stackElement = st.top();
                st.pop();

                while (stackElement.type != OPEN_BRACKET) {
                    if (st.empty()) {
                        throw out_of_range("Invalid expression");
                    }

                    result.push_back(stackElement);
                    stackElement = st.top();
                    st.pop();
                }
                break;
            }

            default: {
                if (!st.empty()) {
                    TOKEN stackElement = st.top();
                    if (stackElement.type == OPEN_BRACKET || stackElement.type == CLOSE_BRACKET) {
                        st.push(currentToken);
                        break;
                    }

                    int pr1 = classToPriority[stackElement.type];
                    int pr2 = classToPriority[currentToken.type];

                    while (pr1 >= pr2) {
                        result.push_back(stackElement);

                        st.pop();
                        if (st.empty()) {
                            break;
                        }

                        stackElement = st.top();
                    }

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

AstNode* AstBuilder::buildAstTree(vector<TOKEN> expression) {
    vector<TOKEN> rpn = buildRpn(expression);
    
    AstNode* root = new AstNode();
    root->token = rpn.back();

    AstNode* currentNode = root;
    for (unsigned long i = rpn.size() - 2; i >= 0; i--) {
        if (rpn[i].type == SYMBOL_CLASS::NUMBER) {
            if (currentNode->left != nullptr) {
                currentNode->right = new AstNode();
                currentNode->right->token = rpn[i];
            } else {
                currentNode->left = new AstNode();
                currentNode->left->token = rpn[i];
            }
        } else {
            currentNode->right = new AstNode();
            currentNode->right->token = rpn[i];
            currentNode = currentNode->right;
        }
    }
    
    return root;
}
