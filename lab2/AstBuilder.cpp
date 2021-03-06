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
    classToPriority[OPEN_BRACKET] = 0;
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

AstNode* AstBuilder::getLastLeftFree(AstNode *root) {
	AstNode* result = root;
	AstNode* currentNode = root;
	while (true) {
		if (currentNode->left == nullptr) {
			result = currentNode;
		}
		if (currentNode->right != nullptr && currentNode->right->token.type != SYMBOL_CLASS::NUMBER) {
			currentNode = currentNode->right;
		}
		else {
			break;
		}
	}
	return result;
}

AstNode* AstBuilder::buildAstTree(vector<TOKEN> expression) {
    vector<TOKEN> rpn = buildRpn(expression);
	stack<AstNode*> stOperations;
    
    AstNode* root = new AstNode();
    root->token = rpn.back();

    AstNode* prevNode = root;

    for (long long i = rpn.size() - 2; i >= 0; i--) {
		AstNode* currentNode = new AstNode();
		currentNode->token = rpn[i];

		while (prevNode->left != nullptr && prevNode->right != nullptr) {
			prevNode = stOperations.top();
			stOperations.pop();
		}

		if (prevNode->right == nullptr) {
			prevNode->right = currentNode;
		}
		else {
			prevNode->left = currentNode;
		}

		if (rpn[i].type != SYMBOL_CLASS::NUMBER) {
			stOperations.push(prevNode);
			prevNode = currentNode;
		}

    }
    
    return root;
}
