#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "grammar_symbols.h"

#define PASCAL_BEGIN "BEGIN"
#define PASCAL_END "END"
#define PASCAL_ELSE "ELSE"
#define PASCAL_IF "IF"
#define PASCAL_WHILE "WHILE"
#define PASCAL_ASSIGNMENT ":="

using namespace std;

enum TYPE_NODE
{
	DATA, NODE_IF, NODE_WHILE, NODE_ASSIGNMENT, NODE_BLOCK
};

struct AstNode {
	TYPE_NODE type = TYPE_NODE::DATA;

	string data;
	vector<AstNode*> list;
};

class AstBuilder
{
private:
	bool doesExpressionReturnsSomething(const string& line);

	bool isAssignmentCorrect(const string& line);
	bool isIfCorrect(const string& line);
	bool isElseCorrect(const string& line);
	bool isWhileCorrect(const string& line);

	GRAMMAR_SYMBOL_TYPE determineGrammarSymbolType(string& line);

	bool isOneLiner(const string& line);
	void makeExpression(const vector<string>& parentExpression, size_t index, vector<string>& returnVector);
	bool checkAndBuildTree(vector<string> lines, AstNode* &root);

public:
	AstBuilder();
	
	AstNode* buildAstTree(vector<string> lines);
	void deleteTree(AstNode* root);
};

