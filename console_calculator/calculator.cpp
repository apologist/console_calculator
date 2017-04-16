#include "calculator.h"

#pragma region Constructors

calculator::calculator(void) {}

calculator::~calculator(void) {}

#pragma endregion

#pragma region Validation

string calculator::format(const string &input) {
  string output;
  string temp = input + "$";  // EOL

  regex white_spaces_2("\\s\\s");
  // delete all whitespaces
  regex white_spaces("\\s");
  temp = regex_replace(temp, white_spaces, "");

  for (int i = 0; i < temp.length(); i++) {
    if (is_sign(temp[i]) || temp[i] == '(' || temp[i] == ')') {
      output.push_back(' ');
      output.push_back(temp[i]);
      output.push_back(' ');
    }
    if (is_digit(temp[i]) || temp[i] == '.') output.push_back(temp[i]);
    if (temp[i] == '$') {
      if (output.back() == ' ')
        output.push_back('$');
      else {
        output.push_back(' ');
        output.push_back(temp[i]);
      }
    }
  }
  // final cleaning
  output = regex_replace(output, white_spaces_2, " ");
  output = trim(output);

  return output;
}

bool calculator::is_valid(const string &input, string &err) {
  string expr_symbols_valid = "[^ 0-9\\.\\+\\-\\*\\/\\(\\)]";
  string expr_spaces_valid = "((\\d|\\.)(\\s+)\\d)";
  regex reg_ex(expr_symbols_valid);
  std::smatch m;
  if (regex_search(input, m, reg_ex)) {
    err = "Invalid character : '" + m.str() + "'.";
    return false;
  }
  reg_ex.assign(expr_spaces_valid);
  if (regex_search(input, m, reg_ex)) {
    err = "Invalid space : '" + m.str() + "'.";
    return false;
  }
  regex white_spaces("\\s");
  string temp = regex_replace(input, white_spaces, "");
  for (int i = 0; i < temp.length() - 1; i++) {
    if (is_sign(temp[i]))
      if (is_sign(temp[i + 1])) {
        string t(1, temp[i]);
        err = "Invalid sign : '" + t + "'.";
        return false;
      }
  }
  return true;
}

#pragma endregion

#pragma region Calculation

double calculator::calculate(const string &input, string &error) {
  string infix = var_substitute(input, error);
  if (error != "") return -1;
  double answer;
  if (is_valid(infix, error)) {
    infix = format(infix);
    string postfix = convert_to_reverse_polish_notation(infix);
    answer = calculate_postfix(postfix);
  } else {
    error = "Invalid expression.";
    return -1;
  }
  return answer;
}

double calculator::calculate_and_show_postfix(const std::string &postf) {
  std::istringstream iss(postf);
  std::vector<double> stack;  // use vector as stack :D
  std::cout << "Input\tOperation\tStack after" << std::endl;
  std::string token;
  while (iss >> token) {
    std::cout << token << "\t";
    double tokenNum;
    if (std::istringstream(token) >> tokenNum) {
      std::cout << "Push\t\t";
      stack.push_back(tokenNum);
    } else {
      std::cout << "Operate\t\t";
      double var_2 = stack.back();
      stack.pop_back();
      double var_1 = stack.back();
      stack.pop_back();
      if (token == "*")
        stack.push_back(var_1 * var_2);
      else if (token == "/")
        stack.push_back(var_1 / var_2);
      else if (token == "-")
        stack.push_back(var_1 - var_2);
      else if (token == "+")
        stack.push_back(var_1 + var_2);
      else if (token == "^")
        stack.push_back(std::pow(var_1, var_2));
      else {
        std::cerr << "Error" << std::endl;
        std::exit(1);
      }
    }
    std::copy(stack.begin(), stack.end(),
              std::ostream_iterator<double>(std::cout, " "));
    std::cout << std::endl;
  }
  return stack.back();
}

double calculator::calculate_postfix(const string &postf) {
  std::istringstream iss(postf);
  std::vector<double> stack;  // use vector as stack :D
  std::string token;
  while (iss >> token) {
    double tokenNum;
    if (std::istringstream(token) >> tokenNum)
      stack.push_back(tokenNum);
    else {
      double var_2 = stack.back();
      stack.pop_back();
      double var_1 = stack.back();
      stack.pop_back();
      if (token == "*")
        stack.push_back(var_1 * var_2);
      else if (token == "/")
        stack.push_back(var_1 / var_2);
      else if (token == "-")
        stack.push_back(var_1 - var_2);
      else if (token == "+")
        stack.push_back(var_1 + var_2);
      else if (token == "^")
        stack.push_back(std::pow(var_1, var_2));
      else {
        std::cerr << "Error!" << std::endl;
        std::exit(1);
      }
    }
  }
  return stack.back();
}

double calculator::calculate_postfix_unsafe(const string &postf) {
  vector<string> post;
  stack<double> stack;
  stack.empty();
  split(postf, ' ', post);
  double var_1, var_2;
  regex rx("^\\d+\.?\\d*$");
  for (int i = 0; i < post.size(); i++) {
    if (regex_match(post[i], rx)) {
      stack.push(stod(post[i]));
    } else if (post[i] == "+") {
      var_1 = stack.top();
      stack.pop();
      var_2 = stack.top();
      stack.pop();
      stack.push(var_1 + var_2);
    } else if (post[i] == "-") {
      var_2 = stack.top();
      stack.pop();
      var_1 = stack.top();
      stack.pop();
      stack.push(var_1 - var_2);
    } else if (post[i] == "*") {
      var_1 = stack.top();
      stack.pop();
      var_2 = stack.top();
      stack.pop();
      stack.push(var_1 * var_2);
    } else if (post[i] == "/") {
      var_2 = stack.top();
      stack.pop();
      var_1 = stack.top();
      stack.pop();
      stack.push(var_1 / var_2);
    } else {
      std::cerr << "Error" << std::endl;
      std::exit(1);
    }
  }
  return stack.top();
}

string calculator::convert_to_reverse_polish_notation(
    const string &inf_string) {
  vector<string> inf, post;
  stack<string> stk;
  split(inf_string, ' ', inf);  // split on whitespaces
  inf = reverse_v(inf);
  int status = 2;  // The variable will have 3 states: 0 - conversion isn't
                   // performed, the error; 1 - conversion process is
                   // successful; 2 - conversion in process
  int i = 0;
  string first;               // stack head
  regex rx("^\\d+\.?\\d*$");  // template for digit
  while (status == 2) {
    if (stk.size() > 0)
      first = stk.top();  // head element
    else
      first = "-1";
    if (regex_match(inf.back(), rx)) {
      post.push_back(inf.back());
      inf.pop_back();
    }
    if (inf.back() == "+" || inf.back() == "-") {
      if (first == "-1" || first == "(") {
        stk.push(inf.back());
        inf.pop_back();
      } else if (first == "+" || first == "-" || first == "*" || first == "/") {
        post.push_back(stk.top());
        stk.pop();
      }
    } else if (inf.back() == "*" || inf.back() == "/") {
      if (first == "-1" || first == "(" || first == "+" || first == "-") {
        stk.push(inf.back());
        inf.pop_back();
      } else if (first == "*" || first == "/") {
        post.push_back(stk.top());
        stk.pop();
      }
    } else if (inf.back() == "(") {
      stk.push(inf.back());
      inf.pop_back();
    } else if (inf.back() == ")") {
      if (first == "-1")
        status = 0;
      else if (first == "+" || first == "-" || first == "*" || first == "/") {
        post.push_back(stk.top());
        stk.pop();
      } else if (first == "(") {
        stk.pop();
        inf.pop_back();
      }
    } else if (inf.back() == "$") {
      // check for the last element
      if (first == "-1")
        status = 1;  // conversion finished
      else if (first == "+" || first == "-" || first == "*" || first == "/") {
        post.push_back(stk.top());
        stk.pop();
      } else if (first == "(")
        status = 0;  // error
    } else
      status = 0;  // unknow symbol
  }
  if (status == 1) {
    string s;
    for (int i = 0; i < post.size(); i++) s += post[i] + " ";
    return s;
  } else
    return "0000";
}

#pragma endregion

#pragma region Variables

bool get_name_and_value(string &input, string &name, string &value) {
  string temp = input.substr(VARIABLE_KEY_WORD.length(),
                             input.length() - VARIABLE_KEY_WORD.length());
  name = temp.substr(0, temp.find('='));
  value = temp.substr(temp.find('=') + 1, temp.length() - temp.find('=') - 1);
  if (name == "" || value == "") return false;
  return true;
}

bool calculator::process_var_line(string &line, string &error) {
  string name;
  string value;
  // regex declare
  string var_valid = VARIABLE_KEY_WORD + "([a-zA-Z_][a-zA-Z0-9_]{0,19})=(.)*?";
  regex reg_var_valid(var_valid);

  regex white_spaces("\\s");

  line = regex_replace(line, white_spaces, "");

  if (regex_match(line, reg_var_valid)) {
    if (!get_name_and_value(line, name, value)) {
      error = "Can't read expression.";
      return false;
    }
  } else {
    error =
        "Invalid variable declaration. Type 'help' to get information about "
        "variable name.";
    return false;
  }
  value = to_string(calculate(value, error));
  if (error != "") {
    error = "Invalid value expression.";
    return false;
  }
  // if(variables[name] == "")
  variables[name] = value;
  return true;
}

void calculator::show_variables() {
  if (variables.size() == 0) {
    std::cout << "You haven't declared any variable." << endl;
    return;
  }
  int p = 0, len = 10;
  string t;
  char sign1 = '-';
  char sign2 = '|';
  char sign3 = '+';
  for (auto var : variables) {
    int f = var.first.length(), s = var.second.length(), m = ((f >= s) ? f : s);
    if (m > len) len = m;
  }
  while (t.length() < len) t.push_back(sign1);

  std::cout << sign3 << setw(t.length()) << t << sign3 << setw(t.length()) << t
            << sign3 << endl;
  std::cout << sign2 << setw(t.length()) << "Name" << sign2 << setw(t.length())
            << "Value" << sign2 << endl;
  std::cout << sign3 << setw(t.length()) << t << sign3 << setw(t.length()) << t
            << sign3 << endl;

  for (auto var : variables) {
    p++;
    std::cout << sign2 << setw(t.length()) << var.first << sign2
              << setw(t.length()) << var.second << sign2 << endl;
    if (p == variables.size())
      std::cout << sign3 << setw(t.length()) << t << sign3 << setw(t.length())
                << t << sign3 << endl;
    else
      std::cout << sign2 << setw(t.length()) << t << sign2 << setw(t.length())
                << t << sign2 << endl;
  }
}

string calculator::var_substitute(const string &input, string &error) {
  // Add spaces validation
  /*string expr_spaces_valid = "((\\d|\\.|\\w|\\_)(\\s+)(\\d|\\w|\\_))";
  std::smatch m;
  regex reg_ex(expr_spaces_valid, regex_constants::basic);
  if(regex_search(input, m, reg_ex))
  {
          error = "Invalid space : '" + m.str() + "'.";
          return "";
  }*/
  string output = divide_spaces(input, error);
  if (error != "") return "";
  vector<string> v;
  split(output, ' ', v);
  for (int i = 0; i < v.size(); i++) {
    if (variables.find(v[i]) != variables.end()) v[i] = variables[v[i]];
  }
  output = "";
  for (auto var : v) {
    output += (var == " ") ? "" : var;
  }
  return output;
}

#pragma endregion

#pragma region Additional functions

template <typename T>
vector<T> calculator::reverse_v(const vector<T> &v1) {
  vector<T> v2;
  size_t i = v1.size();
  while (i > 0) v2.push_back(v1[--i]);
  return v2;
}

void calculator::split(const string &s, char delim, vector<string> &v) {
  auto i = 0;
  auto pos = s.find(delim);
  while (pos != string::npos) {
    v.push_back(s.substr(i, pos - i));
    i = ++pos;
    pos = s.find(delim, pos);

    if (pos == string::npos) v.push_back(s.substr(i, s.length()));
  }
}

string calculator::divide_spaces(const string &input, string &error) {
  string output;
  string temp = input;

  regex white_spaces_2("\\s\\s");
  // delete all whitespaces
  regex white_spaces("\\s");
  temp = regex_replace(temp, white_spaces, "");

  for (int i = 0; i < temp.length(); i++) {
    if (is_sign(temp[i]) || temp[i] == '(' || temp[i] == ')' ||
        temp[i] == ')') {
      output.push_back(' ');
      output.push_back(temp[i]);
      output.push_back(' ');
      continue;
    }
    if (is_digit(temp[i]) || temp[i] == '.' || is_letter(temp[i]) ||
        temp[i] == '_') {
      output.push_back(temp[i]);
      continue;
    }
    error = "Invalid expression.";
    return "";
  }
  // final cleaning
  output = regex_replace(output, white_spaces_2, " ");
  output = trim(output);
  if (output.find(' ') == string::npos) output.push_back(' ');
  return output;
}

bool calculator::is_sign(const char &ch) {
  bool is_sign = (find(signs.begin(), signs.end(), ch) != signs.end());
  return is_sign;
}

bool calculator::is_digit(const char &ch) {
  regex digit_rx("\\d");
  string c(1, ch);
  if (regex_match(c, digit_rx)) return true;
  return false;
}

bool calculator::is_letter(const char &ch) {
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return true;
  return false;
}

string calculator::trim(string &str) {
  str.erase(str.begin(), find_if(str.begin(), str.end(), [](char &ch) -> bool {
              return !isspace(ch);
            }));
  str.erase(find_if(str.rbegin(), str.rend(), [](char &ch) -> bool {
              return !isspace(ch);
            }).base(), str.end());
  return str;
}

void calculator::welcome() { std::cout << WELCOME << endl; }

void calculator::help() {
  for (auto var : HELP_) std::cout << var << endl;
}

void calculator::start() {
  string input;
  string error;
  string expr_var = ".*" + VARIABLE_KEY_WORD + ".*";
  regex regex_var(expr_var);
  std::cout << ">";
  getline(cin, input);
  while (input != "exit") {
    if (input == "help")
      help();
    else if (input == "vars")
      show_variables();
    else if (regex_match(input, regex_var)) {
      if (!process_var_line(input, error)) std::cout << error << endl;
    } else {
      double a = calculate(input, error);
      if (error != "")
        std::cout << error << endl;
      else
        std::cout << to_string(a) << endl;
    }
    std::cout << ">";
    error = "";
    getline(cin, input);
  }
}

#pragma endregion
