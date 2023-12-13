/*
 * Copyright (c) 2023 Harry Chou
 */

#include "./database.h"

Database::Database(std::shared_ptr<spdlog::logger> lg) {
  // setup logger
  logger = lg;
  
  // initializing
  logger->info("Database initializing.");
  open_database();

}

Database::~Database() {
  close_database();
}

void Database::open_database() {
  // open database
  logger->info("Open database - {}", db_name);
  int rc;

  rc = sqlite3_open("test.db", &db);

  if(rc) {
    logger->warn("Can't open database");
  } else {
    logger->info("Opened database successfully");
  }

}

void Database::close_database() {
  // close database
  close_database();
}
