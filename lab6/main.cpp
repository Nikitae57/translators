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
    regex pattern(R"(\s*BE\s*)");
    smatch match;
    regex_match(line, match, pattern);

    if (match.length() == 1) {
        return true;
    }

    string patternStr = R"(^\s*(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))|ID|((-?\d+)))\s*[+-/*%]\s*(((-?\d+(\.\d+)?e[-+]\d+)|(-?\d+\.\d+))|ID|((-?\d+)))$)";
    pattern = regex(patternStr, regex_constants::icase);
    regex_match(line, match, pattern);

    return match.length() > 0;
}

bool isAssignmentCorrect(const string& line) {
    string patternStr = R"(\s*ID\s*:=\s*(ID|BE)\s+)";
    regex patternSimpleAssignment(patternStr);
    smatch match;
    regex_match(line, match, patternSimpleAssignment);

    bool isAssignmentCorrect = match.length() == 1;

    if (!isAssignmentCorrect) {
        size_t assignmentIndex = line.find(PASCAL_ASSIGNMENT);
        string expressionString = line.substr(assignmentIndex + 2);
        isAssignmentCorrect = doesExpressionReturnsSomething(expressionString);
    }

    cout << "Is assignment correct: " << isAssignmentCorrect <<" string: '" << line << '\'' << endl;
    return isAssignmentCorrect;
}

bool isIfCorrect(const string& line) {
    string patternStr = R"(^IF\s*BE\s*THEN\s*$)";
    regex pattern(patternStr);
    smatch match;
    regex_match(line, match, pattern);

    bool isIfCorrect = match.length() == 1;
    cout << "Is IF correct: " << isIfCorrect <<" string: '" << line << '\'' << endl;

    return isIfCorrect;
}

bool isWhileCorrect(const string& line) {
    string patternStr = R"(^\s*WHILE\s*BE\s*DO\s*$)";
    regex pattern(patternStr);
    smatch match;
    regex_match(line, match, pattern);

    bool isWhileCorrect = match.length() == 1;
    cout << "Is WHILE correct: " << isWhileCorrect <<" string: '" << line << '\'' << endl;

    return isWhileCorrect;
}

GRAMMAR_SYMBOL_TYPE determineGrammarSymbolType(string& line) {
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

bool checkSyntax(const vector<string>& lines) {
    bool isCorrect = true;

    size_t i = 0;
    while (i < lines.size()) {
        string currentLine = lines[i];

        GRAMMAR_SYMBOL_TYPE currentType = determineGrammarSymbolType(currentLine);
        switch (currentType) {
            case GRAMMAR_SYMBOL_TYPE::ERROR: { return false; }

            // Assignment doesn't need processing
//            case GRAMMAR_SYMBOL_TYPE::ASSIGNMENT: {
//                isCorrect = processAssignment(currentLine);
//                if (!isCorrect) { break; }
//
//                break;
//            }

            case GRAMMAR_SYMBOL_TYPE::IF: {
                if (i == lines.size() - 1) { return false; }

                vector<string> expression;
                if (lines[i + 1].find(PASCAL_BEGIN) == string::npos) {
                    expression.push_back(lines[++i]);
                } else {
                    while (lines[++i] != PASCAL_END) {
                        if (i == lines.size() - 1) { return false; }
                        expression.push_back(lines[i]);
                    }
                }

                isCorrect = checkSyntax(expression);
                if (!isCorrect) { return false; }

                break;
            }

            case GRAMMAR_SYMBOL_TYPE::WHILE: {
                if (i == lines.size() - 1) { return false; }

                vector<string> expression;
                if (lines[i + 1].find(PASCAL_BEGIN) == string::npos) {
                    expression.push_back(lines[++i]);
                } else {
                    while (lines[++i] != PASCAL_END) {
                        if (i == lines.size() - 1) { return false; }
                        expression.push_back(lines[i]);
                    }
                }

                isCorrect = checkSyntax(expression);
                if (!isCorrect) { return false; }

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