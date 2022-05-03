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
  inline bool operator<(const BigInt &other) const;
  inline BigInt operator=(const BigInt &other) const;
};

bool BigInt::operator<(const BigInt &other) const{
  if (this->digits.size() > other.digits.size()) {
    return false;
  }
  else if (this->digits.size() < other.digits.size()) {
    return true;
  }
  else {
    for (int i = 0; i < this->digits.size(); i++) {
      if (this->digits.get(i) > other.digits.get(i)) {
        return false;
      }
      else if (this->digits.get(i) < other.digits.get(i)) {
        return true;
      }
    }
    return false;
  }
}

BigInt BigInt::operator=(const BigInt &other) const{
  BigInt result;
  for (int i = 0; i < other.digits.size(); i++) {
    result.digits.addLast(other.digits.get(i));
  }
  return result;
}

/** Return the *sum* of this BigInt and the `other` BigInt. */
BigInt BigInt::operator+(const BigInt &other) const {
  // Reference: https://en.wikipedia.org/wiki/Addition
  // Example: 456 + 1123 = 1579
  BigInt result;
  int i = this->digits.size() - 1; int j = other.digits.size() - 1;
  int d1; int d2;
  int sum;
  bool overflow = false;

  while (i >= 0 || j >= 0) {
    if (i < 0) {
      d1 = 0;
    }
    else {
      d1 = this->digits.get(i);
    }
    if (j < 0) {
      d2 = 0;
    }
    else {
      d2 = other.digits.get(j);
    }

    sum = d1 + d2;

    if (overflow) {
      sum++;
      overflow = false;
    }
    if (sum >= 10) {
      sum -= 10;
      overflow = true;
    }

    result.digits.addFirst(sum);
    i--; j--;
  }

  if (overflow) {
    result.digits.addFirst(1);
  }

  return result;
}

/** Return the *absolute difference* between this and the `other` BigInt. */
BigInt BigInt::operator-(const BigInt &other) const {
  // Reference: https://en.wikipedia.org/wiki/Absolute_difference
  // Example: 456 - 1123 = 667
  //Checks to make sure that this is always larger than other
  if (*this < other) {
    return other - *this;
  }

  BigInt result;
  int i = this->digits.size() - 1;
  int j = other.digits.size() - 1;
  int d1; int d2;
  int difference;
  bool overflow = false;

  //Set value to current digit in list unless it cannot be set, which it is 0
  while(i >= 0 || j >= 0) {
    if (i < 0) {
      d1 = 0;
    }
    else {
      d1 = this->digits.get(i);
    }
    if (j < 0) {
      d2 = 0;
    }
    else {
      d2 = other.digits.get(j);
    }

    difference = d1 - d2;
    
    if (overflow) {
      difference -= 1;
      overflow = false;
    }
    if (difference < 0) {
      difference += 10;
      overflow = true;
    }
    
    result.digits.addFirst(difference);

    i--; j--;
  }

  //Remove the any leading zeros
  while(result.digits.getFirst() == 0) {
    //Unless the result is zero
    if (result.digits.size() == 1) {
      return result;
    }
    result.digits.removeFirst();
  }

  return result;
}

}

#endif // __BIGINT_H__