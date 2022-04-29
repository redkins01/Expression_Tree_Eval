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
  ExprTreeNode *root = new ExprTreeNode(tokenStk.top());
  tokenStk.pop();
  if (root->token.is(Token::Kind::Semicolon)) {
    root->expr2 = buildExprTree(tokenStk);
    if (!tokenStk.empty()) {
      root->expr1 = buildExprTree(tokenStk);
    }
  }
  else if (root->token.is_one_of(Token::Kind::Plus, Token::Kind::Minus, Token::Kind::Equal)) {
    root->expr2 = buildExprTree(tokenStk);
    root->expr1 = buildExprTree(tokenStk);
  }
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
  if (root == nullptr) {
    return BigInt();
  }
  if (root->token.is(Token::Kind::Number)) {
    return BigInt(root->token.lexeme);
  }
  else if (root->token.is(Token::Kind::Variable)) {
    return varTbl[root->token.lexeme];
  }
  else if (root->token.is(Token::Kind::Plus)) {
    BigInt op1 = evaluateExprTree(root->expr1, varTbl);
    BigInt op2 = evaluateExprTree(root->expr2, varTbl);
    return op1 + op2;
  }
  else if (root->token.is(Token::Kind::Minus)) {
    BigInt op1 = evaluateExprTree(root->expr1, varTbl);
    BigInt op2 = evaluateExprTree(root->expr2, varTbl);
    return op1 - op2;
  }
  else if (root->token.is(Token::Kind::Equal)) {
    varTbl[root->expr1->token.lexeme] = evaluateExprTree(root->expr2, varTbl);
    return varTbl[root->expr1->token.lexeme];
  }
  else if (root->token.is(Token::Kind::Semicolon)) {
    evaluateExprTree(root->expr1, varTbl);
    return evaluateExprTree(root->expr2, varTbl);
  }
}

} // namespace ds