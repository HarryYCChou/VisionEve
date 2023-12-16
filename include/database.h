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
// User
#include "./user.h"

class Database {
 public:
  // constructor
  Database() {};
  Database(std::shared_ptr<spdlog::logger> logger);
  ~Database();

  // logger
  std::shared_ptr<spdlog::logger> logger;

  // function
  void add_user(User*);
  void del_user();

 private:
  // database
  std::string db_name = "test.db";
  std::string table_name = "patients";
  sqlite3 *db;

  // database function
  void open_database();
  void close_database();
};

#endif  // INCLUDE_DATABASE_H_
