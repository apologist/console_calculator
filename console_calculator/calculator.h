#pragma once

#include <string>
#include <list>
#include <map>
#include <regex>
#include <stack>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <iomanip>
using namespace std;

#pragma region Const

#ifdef _WIN32
    #define NEWLINE "\r\n"
#elif defined macintosh 
    #define NEWLINE "\r"
#else
    #define NEWLINE "\n" 
#endif

const char signs_m[] = {'+', '-', '*', '/'};
const string VARIABLE_KEY_WORD = "var";
const vector<char> signs(signs_m, signs_m + sizeof(signs_m) / sizeof(char));
const string HELP_[] = {"Console calculator help:",
						"- Supported operations : '+', '-', '*', '/', '(', ')';",
						"- To set variable : '" + VARIABLE_KEY_WORD + " <variable_name> = <value>';",
						"- To see declared variables type 'vars';",
						"- Use only letters, digits and '_' to name variables;",
						"- Variable name can start only with a letter or '_';",
						"- Max length of variable name is 20 characters;",
						"- Type 'exit' to finish;",
						"- Source code : https://github.com/apologist/console_calculator.",
						};
const string WELCOME = "Console calculator. Variables are supported. Type expression to calculate. Type 'help' to get help. Type 'vars' to show variables. Type 'exit' to finish.";

#pragma endregion 

class calculator
{
public:
	calculator(void);
	~calculator(void);
	void start();
	double calculate(const string&, string&);
	static void welcome();
	static void help();
private:
	double calculate_postfix(const string&);
	double calculate_postfix_unsafe(const string&);
	double calculate_and_show_postfix(const string&);
	string convert_to_reverse_polish_notation(const string&);
//validation
private:
	string format(const string&);
	bool is_valid(const string&, string&);//only for formated strings
//additional functions
private:
	bool is_sign(const char&);
	bool is_digit(const char&);
	bool is_letter(const char&);
	string trim(string&);
	template <typename T>
	vector<T> reverse_v (const vector<T>&);
	void split(const string&, char, vector<string>&);
//variables
private:
	string divide_spaces(const string&, string&);
	map <string, string> variables;
	bool process_var_line(string&, string&);
	void show_variables();
	string var_substitute(const string&, string&);
};

