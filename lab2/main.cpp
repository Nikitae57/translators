#include <iostream>
#include <fstream>
#include <regex>
#include <map>

using namespace std;

enum TYPES {
    ID, CONST_INT, CONST_FLOAT, ERROR
};

map<TYPES, string> typeToString = {
    {ID, "ID"},
    {CONST_INT, "CONST_INT"},
    {CONST_FLOAT, "CONST_FLOAT"},
    {ERROR, "ERROR"}
};

ifstream inStream;
string yytext;
TYPES currentType;

bool isId(const string& token) {
    bool isId = true;
    regex pattern(R"([a-zA-Z](\d|[a-zA-Z])*)");
    smatch match;
    regex_match(token, match, pattern);

    if (match.length() <= 0) {
        isId = false;
    }

    return isId;
}

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

    string token;
    inStream >> token;
    string s = " s = asd";

    if (token.length() == 1 && token[0] == '\n') {
        return;
    }

    if (isId(token)) {
        yytext = token;
        currentType = ID;
    } else if (isConstInt(token)) {
        yytext = token;
        currentType = CONST_INT;
    } else if (isConstFloat(token)) {
        yytext = token;
        currentType = CONST_FLOAT;
    } else {
        yytext = token;
        currentType = ERROR;
    }
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

    yylex();
    cout << typeToString[currentType] << ':' << yytext;

    switch (currentType) {
        case CONST_INT: {
            int i = stoi(yytext);
            break;
        }

        case CONST_FLOAT: {
            float i = stof(yytext);
            break;
        }

        default: {
            string i = yytext;
        }
    }

    return 0;
}