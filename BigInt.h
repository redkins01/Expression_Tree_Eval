#ifndef __BIGINT_H__
#define __BIGINT_H__

#include "VList.h"
#include <iostream>
#include <string>

namespace ds {

class BigInt {
private:
  VList<int> digits; // stores the digits of the BigInt

public:
  /**
   * Construct a new BigInt object from string.
   *
   * @param val a string of unlimited digits; default is empty string
   */
  explicit BigInt(const std::string &val = "") {
    for (unsigned j = 0; j < val.length(); j++) {
      digits.addLast(val[j] - '0');
    }
  }

  /** Override `<<` to print all digits to os. */
  friend std::ostream &operator<<(std::ostream &os, const BigInt &bi) {
    for (int i = 0; i < bi.digits.size(); i++)
      os << bi.digits.get(i);
    return os;
  }

  inline BigInt operator+(const BigInt &other) const;
  inline BigInt operator-(const BigInt &other) const;
  //inline BigInt operator*(const BigInt &other) const;
};

/** Return the *sum* of this BigInt and the `other` BigInt. */
BigInt BigInt::operator+(const BigInt &other) const {
  // Reference: https://en.wikipedia.org/wiki/Addition
  // Example: 456 + 1123 = 1579

  VList<int> smaller;
  VList<int> larger;
  int s_size;
  int l_size;
  BigInt value;
  int i = 0;
  int num;
  bool overflow = false;

  //Finds the larger of the two numbers (if they are the same the first digit in the addition will be the smaller)
  if (other.digits.size() < this->digits.size()) {
    smaller = other.digits;
    larger = this->digits;
  }
  else {
    smaller = this->digits;
    larger = other.digits;
  }
  //Sets sizes for use in for loops (using smaller.size() leads to error because we are deleting sizes)
  s_size = smaller.size();
  l_size = larger.size();

  //Adds the last digit of each vlaue, if there is an overflow sets value to true and will add to next value in list
  for (; i < s_size; i++) {
    num = smaller.getLast() + larger.getLast();
    if (overflow) {
      num++;
      overflow = false;
    }
    if (num > 9) {
      overflow = true;
    }
    value.digits.addFirst(num % 10);
    smaller.removeLast();
    larger.removeLast();
  }

  //Puts the remaining digits from larger in the list (if there is an over flow, adds it to the next place after smaller ends and sets it to false)
  for (; i < l_size; i++) {
    num = larger.getLast();
    if (overflow) {
      num++;
      overflow = false;
    }
    value.digits.addFirst(num);
    larger.removeLast();
  }

  //If the values of the same length add 1 to the list if there is an overflow
  if (overflow) {
    value.digits.addFirst(1);
    overflow = false;
  }

  return value;
}

/** Return the *absolute difference* between this and the `other` BigInt. */
BigInt BigInt::operator-(const BigInt &other) const {
  // Reference: https://en.wikipedia.org/wiki/Absolute_difference
  // Example: 456 - 1123 = 667

  VList<int> smaller = this->digits;
  VList<int> larger = other.digits;
  int s_size;
  int l_size;
  BigInt value;
  int i = 0;
  int num;

  //Finds the larger of the two numbers (if they are the same the first digit in the subtraction will be the smaller)
  if (other.digits.size() < this->digits.size()) {
    smaller = other.digits;
    larger = this->digits;
  }
  else if (other.digits.size() == this->digits.size()){
    for (int j = 0; j < this->digits.size(); j++) {
      if (this->digits.get(j) > other.digits.get(j)) {
        larger = this->digits;
        smaller = other.digits;
        break;
      }
      else if (this->digits.get(j) < other.digits.get(j)) {
        larger = other.digits;
        smaller = this->digits;
        break;
      }
    }    
  }
  else {
    smaller = this->digits;
    larger = other.digits;
  }
  s_size = smaller.size();
  l_size = larger.size();

  //Subtracts the last two digits. If the last value of larger is smaller than smaller then adds 10 and subtracts the two
  int temp;
  for (; i < s_size; i++) {
    if (smaller.getLast() > larger.getLast()) {
      temp = larger.get(larger.size() - 2) - 1;
      larger.remove(larger.size() - 2);
      larger.insert(temp, larger.size() - 1);
      num = 10 + larger.getLast() - smaller.getLast();
    }
    else {
      num = larger.getLast() - smaller.getLast();
    }
    value.digits.addFirst(num);
    larger.removeLast();
    smaller.removeLast();
  }

  //Adds the remaining of the larger digits to value
  for (; i < l_size; i++) {
    num = larger.getLast();
    value.digits.addFirst(num);
    larger.removeLast();
  }
  
  //If the first digit is a zero removes it
  while (value.digits.getFirst() == 0) {
    //If the answer is 0 we dont want to remove 0
    if (value.digits.size() == 1) {
      return value;
    } 

    value.digits.removeFirst();
  }

  return value;
}

/*BigInt BigInt::operator*(const BigInt &other) const {
  // Example: 10 * 456 = 4560
  // TODO:
  
         
}*/

} // namespace ds

#endif // __BIGINT_H__