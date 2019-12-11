#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>

#include "AstBuilder.h"
#include "main.h"

using namespace std;

void print_Tree(AstNode* p, int level)
{
	if (p)
	{
		if (level) {
			cout << setw(level) << ' ';
		}

		switch (p->type)
		{
		case NODE_ASSIGNMENT:
			cout << ":=" << endl;
			break;

		case NODE_BLOCK:
			cout << "BLOCK" << endl;
			break;

		case NODE_IF:
			cout << "IF" << endl;
			break;

		case NODE_WHILE:
			cout << "WHILE" << endl;
			break;

		default:
			cout << p->data << endl;
			break;
		}

		for (AstNode* node : p->list) {
			print_Tree(node, level + 4);
		}
	}
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
	AstBuilder builder;
	AstNode* root = builder.buildAstTree(lines);

	cout << endl << endl;

	print_Tree(root, 0);

	cout << endl << endl;

    bool isCorrect = root != nullptr;
    if (isCorrect) {
        cout << "Source code is correct" << endl;
    } else {
        cout << "Source code is not correct" << endl;
    }

    return 0;
}