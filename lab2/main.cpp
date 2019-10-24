#include <iostream>
#include <fstream>
#include <regex>
#include <map>

#include "structs.h"
#include "LinkedList.h"
#include "AstBuilder.h"

using namespace std;

map<SYMBOL_CLASS, string> typeToString = {
    {NUMBER, "NUMBER"},
    {OPERATION_ADD, "OPERATION_ADD"},
    {OPERATION_DIV, "OPERATION_DIV"},
    {OPERATION_MUL, "OPERATION_MUL"},
    {OPERATION_SUB, "OPERATION_SUB"},
    {CLOSE_BRACKET, "CLOSE_BRACKET"},
    {OPEN_BRACKET, "OPEN_BRACKET"}
};

ifstream inStream;

LinkedList<TOKEN> list;

bool isConstInt(const string& token) {
    bool isConstInt = true;
    regex pattern(R"(-?\d+)");
    smatch match;
    regex_match(token, match, pattern);

    if (match.length() <= 0) {
        isConstInt = false;
    }

    return isConstInt;
}

bool isOperation(const string &token) {
    bool isOperation = true;
    regex pattern(R"([+-/*])");
    smatch match;
    regex_match(token, match, pattern);

    if (match.length() <= 0) {
        isOperation = false;
    }

    return isOperation;
}

bool isBracket(const string &token) {
    return token.length() == 1 && (token[0] == '(' || token[0] == ')');
}

bool isConstFloat(const string& token) {
    bool isConstFloat = true;
    regex pattern(R"((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))");

    smatch match;
    regex_match(token, match, pattern);

    if (match.length() <= 0) {
        isConstFloat = false;
    }

    return isConstFloat;
}

void yylex() {
    if (inStream.eof()) {
        cout << "Reached end of file!" << endl;
        return;
    }

    string tokenStr;
    inStream >> tokenStr;

    if (tokenStr.length() == 1 && tokenStr[0] == '\n') {
        return;
    }

    TOKEN* token = new TOKEN();
    if (isConstFloat(tokenStr) || isConstInt(tokenStr)) {
        token->type = SYMBOL_CLASS::NUMBER;
        token->sym.num = stof(tokenStr);

    } else if (isOperation(tokenStr)) {
        switch (tokenStr[0]) {
            case '*':
                token->type = SYMBOL_CLASS::OPERATION_MUL;
            break;

            case '/':
                token->type = SYMBOL_CLASS::OPERATION_DIV;
            break;

            case '+':
                token->type = SYMBOL_CLASS::OPERATION_ADD;
            break;

            case '-':
                token->type = SYMBOL_CLASS::OPERATION_SUB;
            break;
        }

        token->sym.special_char = tokenStr[0];
    } else if (isBracket(tokenStr)) {
        switch (tokenStr[0]) {
            case '(':
                token->type = SYMBOL_CLASS::OPEN_BRACKET;
            break;

            case ')':
                token->type = SYMBOL_CLASS::CLOSE_BRACKET;
            break;
        }

        token->sym.special_char = tokenStr[0];
    }

    list.addItem(token);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "Usage: ./lab2 <file_to_parse>";
        exit(0);
    }

    try {
        inStream.open(argv[1]);
    } catch(const ifstream::failure& e) {
        cout << "Can't open file: " << argv[1];
        exit(0);
    }

    {
        int i = 0;
        while (!inStream.eof()) {
            yylex();
            TOKEN* token = list.getItem(i);
            cout << typeToString[token->type] << ':';

            if (token->type == NUMBER) {
                cout << token->sym.num << endl;
            } else {
                cout << token->sym.special_char << endl;
            }

            i++;
        }

        vector<TOKEN> tokenVector;
        for (unsigned long j = 0; j < list.size(); j++) {
            tokenVector.push_back(*list.getItem(j));
        }

        AstBuilder builder = AstBuilder();
        AstNode* astRoot = builder.buildAstTree(tokenVector);
    }

    return 0;
}