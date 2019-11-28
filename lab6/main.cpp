#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

#include "grammar_symbols.h"
#include "main.h"

#define PASCAL_BEGIN "BEGIN"
#define PASCAL_END "END"
#define PASCAL_IF "IF"
#define PASCAL_WHILE "WHILE"
#define PASCAL_ASSIGNMENT ":="

using namespace std;

bool doesExpressionReturnsSomething(const string& line) {
    regex pattern(R"(^(\s*(BE|ID)|(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))|ID|((-?\d+)))\s*)$)");
    smatch match;

    if (regex_match(line, match, pattern)) {
        return true;
    }
    int i = 0/2;

    string patternStr = R"(^\s*(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))|ID|((-?\d+)))\s*[+-/*%]\s*(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))|ID|((-?\d+)))$)";
    pattern = regex(patternStr, regex_constants::icase);

    return regex_match(line, match, pattern);
}

bool isAssignmentCorrect(const string& line) {
    string patternStr = R"(\s*ID\s*:=\s*BE\s*)";
    regex patternSimpleAssignment(patternStr);
    smatch match;

    bool isAssignmentCorrect = regex_match(line, match, patternSimpleAssignment);

    if (!isAssignmentCorrect) {
        patternStr = R"(\s*ID\s*:=\s*(ID|(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))|ID|((-?\d+)))\s*)\s*([+-/*]?\s*(ID|(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))|ID|((-?\d+)))\s*)\s*)*\s*$)";
        regex pattern(patternStr);
        isAssignmentCorrect = regex_match(line, match, pattern);
    }

    cout << "Is assignment correct: " << isAssignmentCorrect <<" string: '" << line << '\'' << endl;
    return isAssignmentCorrect;
}

bool isIfCorrect(const string& line) {
    string patternStr = R"(^IF\s*BE\s*THEN\s*$)";
    regex pattern(patternStr);
    smatch match;

    bool isIfCorrect = regex_match(line, match, pattern);
    cout << "Is IF correct: " << isIfCorrect <<" string: '" << line << '\'' << endl;

    return isIfCorrect;
}

bool isWhileCorrect(const string& line) {
    string patternStr = R"(^\s*WHILE\s*BE\s*DO\s*$)";
    regex pattern(patternStr);
    smatch match;
    bool isWhileCorrect = regex_match(line, match, pattern);

    cout << "Is WHILE correct: " << isWhileCorrect <<" string: '" << line << '\'' << endl;

    return isWhileCorrect;
}

GRAMMAR_SYMBOL_TYPE determineGrammarSymbolType(string& line) {
    // block
    if (line.find(PASCAL_BEGIN) != string::npos) {
        return GRAMMAR_SYMBOL_TYPE::BLOCK;
    }

    // Assignment
    if (line.find(PASCAL_ASSIGNMENT) != string::npos) {
        bool isCorrect = isAssignmentCorrect(line);
        return isCorrect ? GRAMMAR_SYMBOL_TYPE::ASSIGNMENT : GRAMMAR_SYMBOL_TYPE::ERROR;
    }

    // if
    if (line.find(PASCAL_IF) != string::npos) {
        bool isCorrect = isIfCorrect(line);
        return isCorrect ? GRAMMAR_SYMBOL_TYPE::IF : GRAMMAR_SYMBOL_TYPE::ERROR;
    }

    // while
    if (line.find(PASCAL_WHILE) != string::npos) {
        bool isCorrect = isWhileCorrect(line);
        return isCorrect ? GRAMMAR_SYMBOL_TYPE::WHILE : GRAMMAR_SYMBOL_TYPE::ERROR;
    }

    return GRAMMAR_SYMBOL_TYPE::ERROR;
}

bool isOneLiner(const string& line) {
    return line.find(PASCAL_ASSIGNMENT) != string::npos;
}

void makeExpression(const vector<string>& parentExpression, size_t index, vector<string>& returnVector) {
    if (parentExpression[index].find(PASCAL_BEGIN) != string::npos) {
        returnVector.push_back(parentExpression[index]);

        int foundBegins = 1;
        index++;
        while (index != parentExpression.size() && foundBegins != 0) {
            if (parentExpression[index].find(PASCAL_BEGIN)) {
                foundBegins++;
            }

            if (parentExpression[index].find(PASCAL_END)) {
                foundBegins--;
            }

            returnVector.push_back(parentExpression[index]);
            index++;
        }
        return;
    } else if (isOneLiner(parentExpression[index])) {
        returnVector.push_back(parentExpression[index + 1]);
        return;
    } else {
        returnVector.push_back(parentExpression[index]);
        return makeExpression(parentExpression, index + 1, returnVector);
    }
}

bool checkSyntax(const vector<string>& lines) {
    bool isCorrect = true;

    size_t i = 0;
    while (i < lines.size()) {
        string currentLine = lines[i];

        GRAMMAR_SYMBOL_TYPE currentType = determineGrammarSymbolType(currentLine);
        switch (currentType) {
            case GRAMMAR_SYMBOL_TYPE::ERROR: {
                return false;
            }

            case GRAMMAR_SYMBOL_TYPE::BLOCK: {
                // there is BEGIN but no END
                if (lines[lines.size() - 1].find(PASCAL_END) == string::npos) {
                    return false;
                }
                vector<string> expression;
                for (int j = 1; j <= lines.size() - 2; j++) {
                    expression.push_back(lines[j]);
                }

                isCorrect = checkSyntax(expression);
                if (!isCorrect) {
                    return false;
                }
                i += expression.size() + 1;

                break;
            }

            case GRAMMAR_SYMBOL_TYPE::IF: {
                vector<string> expression;
                makeExpression(lines, i + 1, expression);
                isCorrect = checkSyntax(expression);
                if (!isCorrect) { return false; }
                i += expression.size() + 1;

                break;
            }

            case GRAMMAR_SYMBOL_TYPE::WHILE: {
                vector<string> expression;
                makeExpression(lines, i + 1, expression);
                isCorrect = checkSyntax(expression);
                if (!isCorrect) { return false; }
                i += expression.size() + 1;

                break;
            }

            default: {}
        }

        i++;
    }

    return isCorrect;
}

int main() {
    string filePath = "test";
    ifstream input(filePath);

    if (!input.good()) {
        cout << "Can't open source file" << endl;
        exit(-1);
    }

    vector<string> lines;
    string currentLine;
    while (getline(input, currentLine)) {
        if (!currentLine.empty()) {
            lines.push_back(currentLine);
        }
    }

    bool isCorrect = checkSyntax(lines);
    if (isCorrect) {
        cout << "Source code is correct" << endl;
    } else {
        cout << "Source code is not correct" << endl;
    }

    return 0;
}