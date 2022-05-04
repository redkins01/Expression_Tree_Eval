#include "Transformer.h"

namespace ds {

/**
 * Transform the token stream from infix to postfix form.
 *
 * @param infix list of tokens in infix notation
 * @return stack<Token> token stack in postfix notation
 */
stack<Token>
InfixToPostfixTransformer::infixToPostfix(const VList<Token> &infix) {
  VList<Token> temp = infix;
  stack<Token> postfix;
  int index = 0;
  bool flag = false;
  bool assignement = false;
  VList<int> boollist;
  
  //Makes sure we have a valid starting point (i.e the first character is a number, a left paren, or a variable)
  if (temp.getFirst().is_one_of(Token::Kind::Number, Token::Kind::LeftParen, Token::Kind::Variable)) {
    //Start of the algorithem
    while (temp.size() > 0) {
      //Looks if the index is a number, if it is push it to the stack and remove it from the list for the first number.
      //If it is the second number (i.e the second number a part of the list) go back an index
      if (temp.get(index).is_one_of(Token::Kind::Number, Token::Kind::Variable)) {
        postfix.push(temp.remove(index));
        if (flag) {
          index--;
        }
      }
      //Looks if the index is an operation, if it is increase the index and set the flag to true (looking to see if a number needs to be added)
      //If the flag is true, remove it from the list and set it equal to false
      else if (temp.get(index).is_one_of(Token::Kind::Plus, Token::Kind::Minus)) {
        if (flag) {
          postfix.push(temp.remove(index));
          flag = false;
        }
        else {
          index++;
          flag = true;
        }
      }
      else if (temp.get(index).is(Token::Kind::Equal)) {
        if (assignement) {
          postfix.push(temp.remove(index));
          assignement = false;
        }
        else {
          index++;
          assignement = true;
        }
      }
      //Remove the open parenethis and add the status of the flags to a list, set the flag to false
      else if (temp.get(index).is(Token::Kind::LeftParen)) {
        temp.remove(index);
        boollist.addLast(flag);
        flag = false;
      }
      //Remove the closed parenthis and set the flag to the last value. If there was a flag go back a space
      else if (temp.get(index).is(Token::Kind::RightParen)) {
        temp.remove(index);
        flag = boollist.removeLast();
        if (flag) {
          index--;
        }
      }
      else if (temp.get(index).is(Token::Kind::Semicolon)) {
        if (assignement) {
          index--;
        }
        else {
          postfix.push(temp.remove(index));
        }
      }
    }
  }

  //Return the stack
  return postfix;
}

} // namespace ds
