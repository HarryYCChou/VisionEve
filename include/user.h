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
  char last_name[128] = "\0";
  int gender = 0;
  char race[128] = "\0";
  char eye_condition[128] = "\0";
  char eye_color[128] = "\0";
  char note[128] = "\0";

  // function

 private:
};

#endif  // INCLUDE_USER_H_
