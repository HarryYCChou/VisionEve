/*
 * Copyright (c) 2023 Harry Chou
 */

#ifndef INCLUDE_DATABASE_H_
#define INCLUDE_DATABASE_H_

// == c++ system headers ==
#include <string>
// ==   other headers    ==
// spdlog
#include "spdlog/spdlog.h"
// sqlite3
#include <sqlite3.h>

class Database {
 public:
  // constructor
  Database() {};
  Database(std::shared_ptr<spdlog::logger> logger);
  ~Database() {};

  // logger
  std::shared_ptr<spdlog::logger> logger;

  // function

 private:
};

#endif  // INCLUDE_DATABASE_H_
