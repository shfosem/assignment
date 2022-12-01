#include <stack>
#include <iostream>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


const char LEFT_PARENTHESIS = '(';
const char LEFT_BRACE = '{';
const char LEFT_BRACKET = '[';
const char RIGHT_PARENTHESIS = ')';
const char RIGHT_BRACE= '}';
const char RIGHT_BRACKET = ']';

bool check_balanced_parenthesis(string s)
{
	const char LEFT_SMALL = '(';
	const char LEFT_MIDDLE = '{';
	const char LEFT_BIG = '[';
	const char RIGHT_SMALL = ')';
	const char RIGHT_MIDDLE = '}';
	const char RIGHT_BIG = ']';
	
	char parenthesis;
		stack<char> parentheses;
		for(size_t i=0; i <s.length(); i++)
		{
			if (s[i] == '(' || s[i] == '{' || s[i] == '[')
			{ //왼쪽일 경우 stack 에 push
				parenthesis= s[i];
				parentheses.push(parenthesis);
			}
	
			else if (s[i] == ')')// || ins.peek() == RIGHT_MIDDLE || ins.peek() == RIGHT_BIG) 
			{//오른쪽의 경우 무시하고, 짝이 맞는지 확인 맞으면 pop, 틀리면 return false
			
				if (parentheses.empty())
					return false;
				else if (parentheses.top() == '(')
					parentheses.pop();
				else
					return false;
			}
			else if (s[i] == '}')
			{
				if (parentheses.empty())
					return false;
				else if (parentheses.top() == '{')
					parentheses.pop();
				else
					return false;
			}
			else if (s[i] == ']')
			{
				if (parentheses.empty())
					return false;
				else if (parentheses.top() =='[')
					parentheses.pop();
				else
					return false;
			}
			else
				continue;
			
		}
		if (!parentheses.empty())
			return false;
		return true;
}
string infix_to_postfix(string infix);

char calculate_at(char op1, char op2) {
	string line;
	vector<string> operators;
	ifstream myfile;
	char result;
	int x, y;
	myfile.open("operations.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			operators.push_back(line);
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	if (op1 == 'a')
		y = 1;
	else if (op1 == 'b')
		y = 2;
	else if (op1 == 'c')
		y = 3;

	if (op2 == 'a')
		x = 0;
	else if (op2 == 'b')
		x = 2;
	else if (op2 == 'c')
		x = 4;

	result = operators[y][x];

	return result;
}

char calculate_shap(char op1, char op2) {

	string line;
	vector<string> operators;
	ifstream myfile;
	char result;
	int x, y;
	myfile.open("operations.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			operators.push_back(line);
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	if (op1 == 'a')
		y = 5;
	else if (op1 == 'b')
		y = 6;
	else if (op1 == 'c')
		y = 7;

	if (op2 == 'a')
		x = 0;
	else if (op2 == 'b')
		x = 2;
	else if (op2 == 'c')
		x = 4;

	result = operators[y][x];

	return result;
}

char calculate_and(char op1, char op2) {
	string line;
	vector<string> operators;
	ifstream myfile;
	char result;
	int x, y;
	myfile.open("operations.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			operators.push_back(line);
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	
	if (op1 == 'a')
		y = 9;
	else if (op1 == 'b')
		y = 10;
	else if (op1 == 'c')
		y = 11;

	if (op2 == 'a')
		x = 0;
	else if (op2 == 'b')
		x = 2;
	else if (op2 == 'c')
		x = 4;
	
	result = operators[y][x];

	return result;
}

string evaluate(string infix) 
{
	string postfix = infix_to_postfix(infix);
	stack<char> operands;
	string result;
	for (size_t i = 0; i < postfix.length(); i++) {
		if (postfix[i] == 'a' || postfix[i] == 'b' || postfix[i] == 'c')
			operands.push(postfix[i]);
		else {
			char operand1, operand2;
			operand2 = operands.top();
			operands.pop();
			operand1 = operands.top();
			operands.pop();
			switch (postfix[i]) {
			case '&': operands.push(calculate_and(operand1, operand2)); break;
			case '@': operands.push(calculate_at(operand1, operand2)); break;
			case '#': operands.push(calculate_shap(operand1, operand2)); break;
			}
		}	
	}
	result = operands.top();
	operands.pop();
	return result;
}

string infix_to_postfix(string infix) 
{
	bool switch_continue = true;
	string postfix;
	stack<char> operators;

	for (size_t i = 0; i < infix.length(); ++i) {

		
		if (infix[i] == '(' || infix[i] == '{' || infix[i] == '[') // 여는 괄호
			operators.push(infix[i]);

		else if (infix[i] == '@' || infix[i] == '#') { // operator(!empty일떄)
			if(operators.empty())
				operators.push(infix[i]);
			else {
				switch_continue = true;
				//switch 에서 continue 유효하지 않음
				while (switch_continue) {
					switch (operators.top()) {
					case'@': postfix.push_back(operators.top());
						operators.pop();
						if (operators.empty()) {
							operators.push(infix[i]);
							switch_continue = false;
							break;
						}

						continue;
					case'#': postfix.push_back(operators.top());
						operators.pop();
						if (operators.empty()) {
							operators.push(infix[i]);
							switch_continue = false;
							break;
						}
						continue;
					case'&': postfix.push_back(operators.top());
						operators.pop();
						if (operators.empty()) {
							operators.push(infix[i]);
							switch_continue = false;
							break;
						}
						continue;
					default:operators.push(infix[i]); switch_continue = false; break;

					}
				}
			}
		}
		
		
		else if (infix[i] == '&') // operator &
		{
			if(operators.empty())
				operators.push(infix[i]);
			else {
				switch_continue = true;
				while (switch_continue)
					switch (operators.top()) {
					case'&': postfix.push_back(operators.top());
						operators.pop();
						continue;

					default:
						operators.push(infix[i]);
						switch_continue = false;
						break;
					}
			}
		}
		else if (infix[i] == 'a' || infix[i] == 'b' || infix[i] == 'c') // operands
			postfix.push_back(infix[i]);
		else if (infix[i] == ')') {
			switch_continue = true;
			while (switch_continue) {
				switch (operators.top()) {
				case'(':operators.pop();
					switch_continue = false; break;
				default:  postfix.push_back(operators.top());
					operators.pop();
					continue;
				}
			}
		}
		else if (infix[i] == '}') {
			switch_continue = true;
			while (switch_continue) {
				switch (operators.top()) {
				case'{':operators.pop();
					switch_continue = false; break;
				default:  postfix.push_back(operators.top());
					operators.pop();
					continue;
				}
			}
		}
		else if (infix[i] == ']') {
			switch_continue = true;
			while (switch_continue) {
				switch (operators.top()) {
				case'[':operators.pop();
					switch_continue = false; break;
				default:  postfix.push_back(operators.top());
					operators.pop();
					continue;
				}
			}
		}
		else
			continue;
			
	}
	
	while (!operators.empty()) {

		if (operators.top() == '(' || operators.top() == LEFT_BRACE || operators.top() == '[')
		{
			operators.pop();
			continue;
		}

		postfix.push_back(operators.top());
		operators.pop();

	}
	
	return postfix;
}
int main(void)
{
	bool End_of_input = false;
	string infix;
	vector<string> infixes;
	vector<string> results;
	const string END_OF_INPUT = "EOI";
	
	while (!End_of_input) { // 입력받기
	
		getline(cin,infix);
		infixes.push_back(infix);
		if ((infixes.back()).compare(END_OF_INPUT)==0) {
			End_of_input = true;
		}
		
	}
	
	for (size_t i = 0; i < infixes.size(); i++)
	{
		if ((infixes[i]).compare(END_OF_INPUT) == 0)
		{
		}
		else if (!check_balanced_parenthesis(infixes[i]))
			results.push_back("Error!");
		
		else
			results.push_back(evaluate(infixes[i]));
		

	}


	for (size_t i = 0; i < results.size(); i++)
		cout << results[i] << endl;

	return 0;
}


