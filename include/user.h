/*
 * Copyright (c) 2023 Harry Chou
 */

#ifndef INCLUDE_USER_H_
#define INCLUDE_USER_H_

// == c++ system headers ==
#include <string>

class User {
 public:
  // constructor
  User() {};
  ~User() {};

  // data
  char first_name[128] = "\0";

  // function

 private:
};

#endif  // INCLUDE_USER_H_
