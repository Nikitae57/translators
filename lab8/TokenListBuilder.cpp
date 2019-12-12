//
// Created by nikita on 12.12.2019.
//
#include "TokenListBuilder.h"

SYMBOL_CLASS TokenListBuilder::determineSymbolClass(string &word) {
    map<SYMBOL_CLASS, string> classToRegex;
    classToRegex[NUMBER] = R"(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+)|(-?\d+)))";
    classToRegex[BOOLEAN] = R"(TRUE|FALSE)";
    classToRegex[ID] = R"([a-zA-Z](\d|[a-zA-Z])*)";

    classToRegex[OPERATION_ADD] = R"(\+)";
    classToRegex[OPERATION_MUL] = R"(\*)";
    classToRegex[OPERATION_DIV] = R"(/)";
    classToRegex[OPERATION_SUB] = R"(-)";

    classToRegex[OPERATION_AND] = R"(AND)";
    classToRegex[OPERATION_OR] = R"(OR)";
    classToRegex[OPERATION_EQUALS] = R"(==)";
    classToRegex[OPERATION_NOT_EQUALS] = R"(!=)";

    classToRegex[OPEN_BRACKET] = R"(\()";
    classToRegex[CLOSE_BRACKET] = R"(\))";
    classToRegex[SEMICOLON] = R"(;)";

    classToRegex[COMMAND_ASSIGNMENT] = R"(:=)";
    classToRegex[COMMAND_WHILE] = R"(WHILE)";
    classToRegex[COMMAND_DO] = R"(DO)";
    classToRegex[COMMAND_BEGIN] = R"(BEGIN)";
    classToRegex[COMMAND_END] = R"(END)";
    classToRegex[COMMAND_IF] = R"(IF)";
    classToRegex[COMMAND_ELSE] = R"(ELSE)";

    for (const auto& cToReg : classToRegex) {
        regex reg(cToReg.second);
        smatch match;
        bool isRegRight = regex_match(word, match, reg);

        if (isRegRight) {
            return cToReg.first;
        }
    }

    return SYMBOL_CLASS::ERROR;
}

vector<TOKEN> TokenListBuilder::parseInput(vector<string> words) {
    vector<TOKEN> returnVector;
    for (auto & word : words) {
        TOKEN token;
        SYMBOL_CLASS symbolClass = determineSymbolClass(word);

        if (symbolClass == SYMBOL_CLASS::ERROR) {
            cout << "Can't determine token type";
            exit(-1);
        }

        token.type = symbolClass;
        switch (symbolClass) {
            case NUMBER: {
                token.sym.num = atof(word.c_str());
                break;
            }

            case BOOLEAN: {
                token.sym.boolean_val = word == "TRUE";
                break;
            }

            default: {
                memcpy(token.sym.str_val, word.c_str(), sizeof(char) * 64);
                break;
            }
        }

        returnVector.push_back(token);
    }

    return returnVector;
}

vector<TOKEN> TokenListBuilder::buildTokenList(const string& fileName) {
    fstream input(fileName);
    if (!input.good()) {
        cout << "Can't open file!";
        exit(-1);
    }

    vector<string> words;
    while (!input.eof()) {
        string tmp;
        input >> tmp;
        words.push_back(tmp);
    }

    vector<TOKEN> tokenList = parseInput(words);

    return tokenList;
}