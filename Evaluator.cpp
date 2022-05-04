#include "Evaluator.h"

namespace ds {


/**
 * Create an expression tree from a stack of tokens in postifx form
 *
 * @param tokenStk the postfix tokens; may contain variables, numbers, plus,
 * minus, equal, and semicolon
 * @return ExprTreeNode* the root of the generated expression tree
 */
ExprTreeNode *Evaluator::buildExprTree(std::stack<Token> &tokenStk) {
  //Create a new node
  ExprTreeNode *root = new ExprTreeNode(tokenStk.top());
  tokenStk.pop();
  //If the node is a semi-colon store value to right, and check if a value needs to be stored on the left
  if (root->token.is(Token::Kind::Semicolon)) {
    root->expr2 = buildExprTree(tokenStk);
    if (!tokenStk.empty()) {
      root->expr1 = buildExprTree(tokenStk);
    }
  }
  //If the token is Plus, Minus, or Equals, there will always be two nodes that follow
  else if (root->token.is_one_of(Token::Kind::Plus, Token::Kind::Minus, Token::Kind::Equal)) {
    root->expr2 = buildExprTree(tokenStk);
    root->expr1 = buildExprTree(tokenStk);
  }

  //Implicit number and variable nodes will be stored at the bottom of the tree

  return root;
}

/**
 * Evaluate the given expression tree
 *
 * @param root the root of the expression tree
 * @param varTbl the map from a variable to its value
 * @return BigInt the result of the expression; if the tree is empty, simply
 * return an empty BigInt
 */
BigInt Evaluator::evaluateExprTree(const ExprTreeNode *root,
                                   std::map<std::string, BigInt> &varTbl) {
  //Base case
  if (root == nullptr) {
    return BigInt();
  }
  //If the node is a number just return the value of the number
  if (root->token.is(Token::Kind::Number)) {
    return BigInt(root->token.lexeme);
  }
  //If the node is a variable return the value of the variable from the hash map
  else if (root->token.is(Token::Kind::Variable)) {
    return varTbl[root->token.lexeme];
  }
  //If the node is a plus traverse the left branch and the right branch and return the summed value
  else if (root->token.is(Token::Kind::Plus)) {
    BigInt op1 = evaluateExprTree(root->expr1, varTbl);
    BigInt op2 = evaluateExprTree(root->expr2, varTbl);
    return op1 + op2;
  }
  //If the node is a minus traverse the left branch and the right branch and return the subtracted value
  else if (root->token.is(Token::Kind::Minus)) {
    BigInt op1 = evaluateExprTree(root->expr1, varTbl);
    BigInt op2 = evaluateExprTree(root->expr2, varTbl);
    return op1 - op2;
  }
  //If the node is an equals set the hash map for the variable on the left to the value on the right; return the value from the hash map
  else if (root->token.is(Token::Kind::Equal)) {
    varTbl[root->expr1->token.lexeme] = evaluateExprTree(root->expr2, varTbl);
    return varTbl[root->expr1->token.lexeme];
  }
  //If the node is a semicolon traverse the left path, then return hte right path
  else if (root->token.is(Token::Kind::Semicolon)) {
    evaluateExprTree(root->expr1, varTbl);
    return evaluateExprTree(root->expr2, varTbl);
  }
  //Exhaustive if statement
  return BigInt();
}

} // namespace ds