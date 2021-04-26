#include "abstractsyntaxtree.h"
/**
 * Calculates the value from an AST (Abstract Syntax Tree). To parse numbers from strings, please use std::stod
 * @return A double representing the calculated value from the expression transformed into an AST
 */
double AbstractSyntaxTree::eval() const {
    return calculate(root);

}

double AbstractSyntaxTree::calculate(typename BinaryTree<std::string>::Node* current) const{
  double left_result = 0;
  double right_result = 0;
  if (current->left != NULL) {
    left_result = calculate(current->left);
  }
  if (current->right != NULL) {
    right_result = calculate(current->right);
  }

  if (!isNumber(current->elem)) {
    return (domath(left_result, right_result, current->elem));
  }
  else {
    return stod(current->elem);
  }
}

double AbstractSyntaxTree::domath(double l, double r, string op) const{
  if (op == "+") {
    return l+r;
  }
  else if (op == "-") {
    return l-r;
  }
  else if (op == "*") {
    return l*r;
  }
  else {
    return l/r;
  }
}

// Following code has been inspired from a piece of code found on stackoverflow after the search "isnumeric c++" on google
bool AbstractSyntaxTree::isNumber(const string& s) const{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
