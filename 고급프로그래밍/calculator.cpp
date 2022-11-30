/*
	calculator_buggy.cpp
*/

#include "std_lib_facilities.h"
// made a type Token
struct Token { 
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), name(n) { }
};

class Token_stream {
	bool full;
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void putback(Token t) { buffer=t; full=true; }
	void ignore(char);
	
};
//make a name various Token;
const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char root= '#';
const char calculate_power = '^';
const char c_sin = 'S'; // calculate_sin
const char c_cos = 'C';
const char c_tan = 'T';


//get Token from user
Token Token_stream::get()
{
	if (full) { full=false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
	case '!':
	case '#':
	case '^':
		return Token(ch);
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{	
		cin.unget();
		double val;
		cin >> val;
		return Token(number,val);
	}
	default: //
		if (isalpha(ch)) {
			string s;
			s += ch;
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
			cin.unget();
			if (s == "let") return Token(let);	
			if (s == "quit") return Token(quit);
			if (s == "sqrt") return Token(root);
			if (s == "sin")return Token(c_sin);
			if (s == "cos")return Token(c_cos);
			if (s == "tan")return Token(c_tan);
			return Token(name,s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c==buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin>>ch)
		if (ch==c) return;
}

//make type Variable
struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

// make vector Variables
vector<Variable> names;


double get_value(string s)//print value
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ",s);
}

void set_value(string s, double d)//set value
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ",s);
}


bool is_declared(string s) //check if the variable declared before
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts;

double expression();


int factorial(int n) //made a function to calculate factorial
{
	if (n == 0) // case : 0 return 1;
		return 1;
	else
		return n * factorial(n - 1);

}
double power(double d, int exponent)
{
	double power_result=1;
	
	if (exponent == 0)
		return power_result;
	else if (exponent > 0)
		for (int i = 0; i < exponent; ++i)
			power_result *= d;
	else if (exponent < 0)
	for (int i = 0; i > exponent; --i)
			power_result /= d;
	return power_result;

}


double primary()
{

	Token t = ts.get();
	switch (t.kind) {

	case '(':
	{	double d = expression();
	t = ts.get();
	if (t.kind != ')') error("')' expected");
	return d;// fix the code
	}
	case '-':
		return -primary();
	case '+': //unary +
		return +primary();
	case name:
		return get_value(t.name);
	case number:
		return t.value;
	case root: { //case root
		double d = expression();
		if (d < 0) error("positive expeted");
		d = sqrt(d);
		return d;
	}
	
	case c_sin: { // case sin
		double d = expression();
		d = sin(d);
		return d;
	}
	case c_cos: { //case cos
		double d = expression();
		d = cos(d);
		return d;
	}
	case c_tan: { //case tan
		double d = expression();
		d = tan(d);
		return d;
	}
	
	default:
		error("primary expected");


	}
}

double checkfactorial()// check factorial , if factorial calculate else call primary

{
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		
		case '!':
			left = factorial(left);
			break;
		case '^':
		{double d = primary();
		left = power(left, d);
		break; }
		default:
			ts.putback(t);
			return left;
		}
	}
}

double term()//*,/ calculate
{
	double left = checkfactorial();
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '*':
			left *= checkfactorial();
			break;
		case '/':
		{	double d = checkfactorial();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}

		default:
			ts.putback(t);
			return left;
		}
	}
}

double expression() //+,- calculate
{
	double left = term();
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
}

double declaration()
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,name);
	double d = expression();
	names.push_back(Variable(name,d));
	return d;
}


void declaration_constant()// make a fuction that declares constants
{
	const Variable const1("pi", 3.14159);
	const Variable const2("e", 2.71828);
	names.push_back(const1);
	names.push_back(const2);
	
}

double statement()
{
	Token t = ts.get();
	switch(t.kind) {
	case let:
		return declaration();

	default:
		ts.putback(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	
	while(true) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t=ts.get();
		if (t.kind == quit) return;
		ts.putback(t);
		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()
try {
	declaration_constant();// declaration constant
	calculate();
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	return 1;
}
catch (...) {
	cerr << "exception\n";
	return 2;
}
