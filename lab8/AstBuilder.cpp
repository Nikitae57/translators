//
// Created by nikita on 12.12.2019.
//

#include "AstBuilder.h"

using namespace std;

bool AstBuilder::checkAndBuildExprTree(vector<TOKEN> tokens, AstNode *&root) {
    vector<TOKEN> rpn;
    stack<AstNode*> stOperations;
    try {
        rpn = buildExprRpn(tokens);
    } catch (exception &e) {
        return false;
    }

    root = new AstNode();
    root->exprToken = rpn.back();
    AstNode* prevNode = root;

    prevNode->children.push_back(nullptr);
    prevNode->children.push_back(nullptr);
    for (long long i = rpn.size() - 2; i >= 0; i--) {

        auto currentNode = new AstNode();
        currentNode->exprToken = rpn[i];

        while (prevNode->children[0] != nullptr && prevNode->children[1] != nullptr) {
            prevNode = stOperations.top();
            stOperations.pop();
        }

        if (prevNode->children[1] == nullptr) {
            prevNode->children[1] = currentNode;
        } else {
            prevNode->children[0] = currentNode;
        }

        if (rpn[i].type != SYMBOL_CLASS::NUMBER) {
            stOperations.push(prevNode);
            prevNode = currentNode;
            prevNode->children.push_back(nullptr);
            prevNode->children.push_back(nullptr);
        }
    }
    root->exprToken = rpn.back();

    return true;
}

bool AstBuilder::checkAndBuildBooleanExprTree(vector<TOKEN> tokens, AstNode *&root) {
    vector<TOKEN> rpn;
    stack<AstNode*> stOperations;
    try {
        rpn = buildBoolExprRpn(tokens);
    } catch (exception &e) {
        return false;
    }

    root = new AstNode();
    root->exprToken = rpn.back();
    AstNode* prevNode = root;

    prevNode->children.push_back(nullptr);
    prevNode->children.push_back(nullptr);
    for (long long i = rpn.size() - 2; i >= 0; i--) {
        auto currentNode = new AstNode();
        currentNode->exprToken = rpn[i];

        while (prevNode->children[0] != nullptr && prevNode->children[1] != nullptr) {
            prevNode = stOperations.top();
            stOperations.pop();
        }

        if (prevNode->children[1] == nullptr) {
            prevNode->children[1] = currentNode;
        } else {
            prevNode->children[0] = currentNode;
        }

        if (rpn[i].type != SYMBOL_CLASS::NUMBER) {
            stOperations.push(prevNode);
            prevNode = currentNode;
            prevNode->children.push_back(nullptr);
            prevNode->children.push_back(nullptr);
        }
    }
    root->exprToken = rpn.back();

    return true;
}

vector<TOKEN> AstBuilder::buildBoolExprRpn(vector<TOKEN> &tokens) {
    vector<SYMBOL_CLASS> allowedTokens;
    allowedTokens.push_back(SYMBOL_CLASS::OPEN_BRACKET);
    allowedTokens.push_back(SYMBOL_CLASS::CLOSE_BRACKET);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_AND);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_OR);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_EQUALS);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_NOT_EQUALS);
    allowedTokens.push_back(SYMBOL_CLASS::BOOLEAN);

    for (auto &token : tokens) {
        auto isTokenAllowed = false;
        for (auto &allowedToken : allowedTokens) {
            if (token.type == allowedToken) {
                isTokenAllowed = true;
                break;
            }
        }

        if (!isTokenAllowed) {
            throw runtime_error("Incorrect expression");
        }
    }

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
    vector<SYMBOL_CLASS> allowedTokens;
    allowedTokens.push_back(SYMBOL_CLASS::OPEN_BRACKET);
    allowedTokens.push_back(SYMBOL_CLASS::CLOSE_BRACKET);
    allowedTokens.push_back(SYMBOL_CLASS::NUMBER);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_DIV);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_MUL);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_ADD);
    allowedTokens.push_back(SYMBOL_CLASS::OPERATION_SUB);

    for (auto &token : tokens) {
        auto isTokenAllowed = false;
        for (auto &allowedToken : allowedTokens) {
            if (token.type == allowedToken) {
                isTokenAllowed = true;
                break;
            }
        }

        if (!isTokenAllowed) {
            throw runtime_error("Incorrect expression");
        }
    }

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
    // Only assignment can be oneliner. It's always starts with ID
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

            case SYMBOL_CLASS::COMMAND_BEGIN: {
                vector<TOKEN> blockContent;
                makeBlockContent(tokens, i, blockContent);
                i += blockContent.size() + 1;

                // Remove first BEGIN and last END (it's belong to parent block)
                blockContent.erase(blockContent.begin());
                blockContent.erase(blockContent.begin() + blockContent.size() - 1);

                result->type = AST_NODE_TYPE::BLOCK;
                checkAndBuildTree(blockContent, root);

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

                // Get token after assignment ':='
                i += 2;
                vector<TOKEN> exprTokens;
                while (tokens[i].type != SYMBOL_CLASS::SEMICOLON) {
                    exprTokens.push_back(tokens[i++]);
                }

                isCorrect = checkAndBuildExprTree(exprTokens, result->children.back());
                if (!isCorrect) { return false; }
                result->children.back()->type = AST_NODE_TYPE::ARITHMETIC_EXPR;

                break;
            }

            case SYMBOL_CLASS::COMMAND_IF: {
                result->type = AST_NODE_TYPE::IF;
                vector<TOKEN> boolExprTokens;
                if (i == tokens.size() - 2) {
                    return false;
                }

                // Get boolean expression
                while (tokens[++i].type != SYMBOL_CLASS::COMMAND_THEN) {
                    if (i == tokens.size() - 2) {
                        return false;
                    }
                    boolExprTokens.push_back(tokens[i]);
                }

                result->children.push_back(nullptr);
                isCorrect = checkAndBuildBooleanExprTree(boolExprTokens, result->children.back());
                if (!isCorrect) { return false; }
                result->children.back()->type = AST_NODE_TYPE::BOOL_EXPR;

                // Get THEN content
                result->children.push_back(nullptr);
                vector<TOKEN> thenContent;
                makeBlockContent(tokens, i + 1, thenContent);
                isCorrect = checkAndBuildTree(thenContent, result->children.back());
                if (!isCorrect) { return false; }

                i += thenContent.size() + 1;
                result->children.push_back(nullptr);
                if (tokens[i].type == SYMBOL_CLASS::COMMAND_ELSE) {
                    vector<TOKEN> elseContent;
                    makeBlockContent(tokens, i + 1, elseContent);
                    isCorrect = checkAndBuildTree(elseContent, result->children.back());

                    if (!isCorrect) { return false; }
                    i += elseContent.size() + 1;
                }

                break;
            }

            default: {
            }
        }

        i++;
    }

    return isCorrect;
}

AstNode* AstBuilder::buildAstTree(vector<TOKEN>& tokens) {
    AstNode* result = nullptr;
    if (!checkAndBuildTree(tokens, result)) {
        deleteTree(result);
        result = nullptr;
    }

    return result;
}