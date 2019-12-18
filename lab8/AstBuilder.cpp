//
// Created by nikita on 12.12.2019.
//

#include "AstBuilder.h"

using namespace std;

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

void AstBuilder::makeOneLiner(
    const vector<TOKEN>& parentExpression,
    size_t index,
    vector<TOKEN>& returnVector
) {
    while (parentExpression[index].type != SYMBOL_CLASS::SEMICOLON) {
        returnVector.push_back(parentExpression[index++]);
    }
    returnVector.push_back(parentExpression[index]);
}

bool isOneLiner(const TOKEN& token) {
    // Only assignment can be one liner. It's always starts with ID
    return token.type == SYMBOL_CLASS::ID;
}

// Is used for making content of BEGIN-END block of every
// complex expression (IF, ELSE, WHILE)
void AstBuilder::makeBlockContent(
    const vector<TOKEN>& parentExpression,
    size_t index,
    vector<TOKEN>& returnVector
) {
    if (parentExpression[index].type == SYMBOL_CLASS::COMMAND_BEGIN) {
        returnVector.push_back(parentExpression[index]);

        int foundBegins = 1;
        index++;
        while (index != parentExpression.size() && foundBegins != 0) {
            if (parentExpression[index].type == SYMBOL_CLASS::COMMAND_BEGIN) {
                foundBegins++;
            }

            if (parentExpression[index].type == SYMBOL_CLASS::COMMAND_END) {
                foundBegins--;
            }

            returnVector.push_back(parentExpression[index]);
            index++;
        }
        return;
    } else if (isOneLiner(parentExpression[index])) {
        makeOneLiner(parentExpression, index, returnVector);
        return;
    } else {
        returnVector.push_back(parentExpression[index]);
        return makeBlockContent(parentExpression, index + 1, returnVector);
    }
}

bool AstBuilder::checkAndBuildTree(vector<TOKEN> tokens, AstNode *&root) {
    bool isCorrect = true;
    std::size_t i = 0;
    while (i < tokens.size()) {
        auto* result = new AstNode();

        if (root != nullptr) {
            root->children.push_back(result);
        } else {
            root = result;
        }

        TOKEN currentToken = tokens[i];
        switch (currentToken.type) {
            case SYMBOL_CLASS::ERROR: {
                cout << "Bad token: " << currentToken.sym.str_val;
                break;
            }

            case SYMBOL_CLASS::COMMAND_WHILE: {
                result->type = AST_NODE_TYPE::WHILE;
                vector<TOKEN> boolExpr;
                if (i == tokens.size() - 1) {
                    return false;
                }

                while (tokens[++i].type != SYMBOL_CLASS::COMMAND_DO) {
                    boolExpr.push_back(tokens[i]);
                    if (i == tokens.size() - 1) {
                        return false;
                    }
                }

                auto exprRoot = new AstNode();
                checkAndBuildBooleanExprTree(boolExpr, exprRoot);
                if (exprRoot == nullptr) {
                    return false;
                }
                
                exprRoot->type = AST_NODE_TYPE::BOOL_EXPR;
                result->children.push_back(exprRoot);
                result->children.push_back(nullptr);

                vector<TOKEN> whileContent;
                makeBlockContent(tokens, i + 1, whileContent);
                isCorrect = checkAndBuildTree(whileContent, result->children.back());
                if (!isCorrect) { return false; }
                i += whileContent.size();

                break;
            }

            // Assignment starts with ID
            case SYMBOL_CLASS::ID: {
                result->type = AST_NODE_TYPE::ASSIGNMENT;

                auto variable = new AstNode();
                variable->type = AST_NODE_TYPE::ID_AST;
                result->children.push_back(variable);
                result->children.push_back(nullptr);

                i += 2;
                vector<TOKEN> exprTokens;
                while (tokens[i].type != SYMBOL_CLASS::SEMICOLON) {
                    exprTokens.push_back(tokens[i++]);
                }
                i++;

                isCorrect = checkAndBuildExprTree(exprTokens, result->children.back());
                if (!isCorrect) {
                    return false;
                }

                break;
            }

            case SYMBOL_CLASS::COMMAND_IF: {

                vector<TOKEN> exprTokens;
                while (tokens[++i].type != SYMBOL_CLASS::COMMAND_THEN) {
                    exprTokens.push_back(tokens[i]);
                }

                result->children.push_back(nullptr);
                isCorrect = checkAndBuildBooleanExprTree(exprTokens, result->children.back());
                if (!isCorrect) {
                    return false;
                }

                result->children.push_back(nullptr);
                vector<TOKEN> thenContent;
                makeBlockContent(tokens, i + 1, thenContent);
                isCorrect = checkAndBuildTree(thenContent, result->children.back());

                if (!isCorrect) {
                    return false;
                }

                i += thenContent.size();
                result->children.push_back(nullptr);
                if (tokens[i + 1].type == SYMBOL_CLASS::COMMAND_ELSE) {
                    vector<TOKEN> elseContent;
                    makeBlockContent(tokens, i + 1, elseContent);
                    isCorrect = checkAndBuildTree(elseContent, result->children.back());

                    if (!isCorrect) {
                        return false;
                    }

                    i += elseContent.size();
                }

                break;
            }

            default: {}
        }

    }

    return isCorrect;
}

AstNode* AstBuilder::buildAstTree(vector<TOKEN> tokens) {
    AstNode* result = nullptr;
    if (!checkAndBuildTree(tokens, result)) {
        deleteTree(result);
        result = nullptr;
    }

    return result;
}