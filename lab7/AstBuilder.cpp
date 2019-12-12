#include "AstBuilder.h"

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
	return ltrim(rtrim(str, chars), chars);
}

bool AstBuilder::checkAndBuildTree(vector<string> lines, AstNode* &root)
{
	bool isCorrect = true;

	size_t i = 0;
	while (i < lines.size()) {
		AstNode* result = new AstNode();

		if (root != nullptr) {
			root->list.push_back(result);
		}
		else {
			root = result;
		}

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
			for (size_t j = 1; j <= lines.size() - 2; j++) {
				expression.push_back(lines[j]);
			}

			root->type = TYPE_NODE::NODE_BLOCK;

			isCorrect = checkAndBuildTree(expression, root);
			if (!isCorrect) {
				return false;
			}
			i += expression.size() + 1;

			break;
		}

		case GRAMMAR_SYMBOL_TYPE::IF: {
			vector<string> expression;
			makeExpression(lines, i + 1, expression);

			result->type = TYPE_NODE::NODE_IF;

			AstNode* be = new AstNode();
			be->data = "BE";
			result->list.push_back(be);

			result->list.push_back(nullptr);
			isCorrect = checkAndBuildTree(expression, result->list.back());
			if (!isCorrect) { return false; }
			i += expression.size();
			if (lines.size() > (i + 1)) {
				string nextString = lines[i + 1];
				if (determineGrammarSymbolType(nextString) == GRAMMAR_SYMBOL_TYPE::ELSE) {
					i++;
					expression.clear();
					makeExpression(lines, i + 1, expression);

					result->list.push_back(nullptr);

					isCorrect = checkAndBuildTree(expression, result->list.back());
					if (!isCorrect) { return false; }
					i += expression.size();
				}
			}
			break;
		}

		case GRAMMAR_SYMBOL_TYPE::ELSE: {
			return false;
			break;
		}

		case GRAMMAR_SYMBOL_TYPE::WHILE: {
			vector<string> expression;
			makeExpression(lines, i + 1, expression);

			result->type = TYPE_NODE::NODE_WHILE;
			 
			AstNode* be = new AstNode();
			be->data = "BE";
			result->list.push_back(be);

			result->list.push_back(nullptr);
			isCorrect = checkAndBuildTree(expression, result->list.back());
			if (!isCorrect) { return false; }
			i += expression.size();

			break;
		}

		case GRAMMAR_SYMBOL_TYPE::ASSIGNMENT: {
			result->type = TYPE_NODE::NODE_ASSIGNMENT;

			AstNode* data = new AstNode();
			data->data = "ID";
			result->list.push_back(data);

			data = new AstNode();

			size_t pos = currentLine.find(PASCAL_ASSIGNMENT);
			string rightPart = currentLine.substr(pos + 2);
			trim(rightPart);

			data->data = rightPart;
			result->list.push_back(data);

			break;
		}

		default: {}
		}

		i++;
	}

	return isCorrect;
}

void AstBuilder::deleteTree(AstNode* root)
{
	if (root == nullptr) {
		return;
	}

	for (AstNode* node : root->list) {
		deleteTree(node);
	}

	delete root;
}

AstBuilder::AstBuilder()
{
}

AstNode* AstBuilder::buildAstTree(vector<string> lines)
{
	AstNode* result = nullptr;
	if (!checkAndBuildTree(lines, result)) {
		deleteTree(result);
		result = nullptr;
	}

	return result;
}

bool AstBuilder::doesExpressionReturnsSomething(const string& line)
{
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

bool AstBuilder::isAssignmentCorrect(const string& line)
{
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

bool AstBuilder::isIfCorrect(const string& line)
{
	string patternStr = R"(^\s*IF\s*BE\s*THEN\s*$)";
    regex pattern(patternStr);
    smatch match;

    bool isIfCorrect = regex_match(line, match, pattern);
    cout << "Is IF correct: " << isIfCorrect <<" string: '" << line << '\'' << endl;

    return isIfCorrect;
}

bool AstBuilder::isElseCorrect(const string& line)
{
	string patternStr = R"(^\s*ELSE\s*$)";
	regex pattern(patternStr);
	smatch match;

	bool isElseCorrect = regex_match(line, match, pattern);
	cout << "Is ELSE correct: " << isElseCorrect << " string: '" << line << '\'' << endl;

	return isElseCorrect;
}

bool AstBuilder::isWhileCorrect(const string& line)
{
	 string patternStr = R"(^\s*WHILE\s*BE\s*DO\s*$)";
    regex pattern(patternStr);
    smatch match;
    bool isWhileCorrect = regex_match(line, match, pattern);

    cout << "Is WHILE correct: " << isWhileCorrect <<" string: '" << line << '\'' << endl;

    return isWhileCorrect;
}

GRAMMAR_SYMBOL_TYPE AstBuilder::determineGrammarSymbolType(string& line)
{
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

	// else
	if (line.find(PASCAL_ELSE) != string::npos) {
		bool isCorrect = isElseCorrect(line);
		return isCorrect ? GRAMMAR_SYMBOL_TYPE::ELSE : GRAMMAR_SYMBOL_TYPE::ERROR;
	}

    // while
    if (line.find(PASCAL_WHILE) != string::npos) {
        bool isCorrect = isWhileCorrect(line);
        return isCorrect ? GRAMMAR_SYMBOL_TYPE::WHILE : GRAMMAR_SYMBOL_TYPE::ERROR;
    }

    return GRAMMAR_SYMBOL_TYPE::ERROR;
}

bool AstBuilder::isOneLiner(const string& line)
{
	return line.find(PASCAL_ASSIGNMENT) != string::npos;
}

void AstBuilder::makeExpression(const vector<string>& parentExpression, size_t index, vector<string>& returnVector)
{
	 if (parentExpression[index].find(PASCAL_BEGIN) != string::npos) {
        returnVector.push_back(parentExpression[index]);

        int foundBegins = 1;
        index++;
        while (index != parentExpression.size() && foundBegins != 0) {
            if (parentExpression[index].find(PASCAL_BEGIN) != string::npos) {
                foundBegins++;
            }

            if (parentExpression[index].find(PASCAL_END) != string::npos) {
                foundBegins--;
            }

            returnVector.push_back(parentExpression[index]);
            index++;
        }
        return;
    } else if (isOneLiner(parentExpression[index])) {
        returnVector.push_back(parentExpression[index]);
        return;
    } else {
        returnVector.push_back(parentExpression[index]);
        return makeExpression(parentExpression, index + 1, returnVector);
    }
}
