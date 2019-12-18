//
// Created by nikita on 12.12.2019.
//

#ifndef LAB8_TOKENLISTBUILDER_H
#define LAB8_TOKENLISTBUILDER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>

using namespace std;

enum SYMBOL_CLASS {
    ERROR,
    NUMBER, BOOLEAN,
    OPERATION_MUL, OPERATION_DIV, OPERATION_ADD, OPERATION_SUB,
    OPERATION_AND, OPERATION_OR, OPERATION_EQUALS, OPERATION_NOT_EQUALS,
    OPEN_BRACKET, CLOSE_BRACKET, SEMICOLON,
    COMMAND_WHILE, COMMAND_IF, COMMAND_ELSE, COMMAND_DO, COMMAND_THEN,
    COMMAND_BEGIN, COMMAND_END, COMMAND_ASSIGNMENT,
    ID
};

union SYMBOL_VALUE {
    float num;
    bool boolean_val;
    char str_val[64];
};

struct TOKEN {
    SYMBOL_CLASS type;
    SYMBOL_VALUE sym;
};

class TokenListBuilder {
private:
    vector<TOKEN> parseInput(vector<string> words);
    SYMBOL_CLASS determineSymbolClass(string& word);

public:
    vector<TOKEN> buildTokenList(const string& fileName);
};

#endif //LAB8_TOKENLISTBUILDER_H
