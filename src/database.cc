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

void Database::add_user(User* u) {
  const char* sql_insert_data =
    "INSERT INTO employees (first_name, last_name, age, gender, race, eye_color, medical_condition, eye_condition, user_note) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

  // Prepare the SQL statement
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, sql_insert_data, -1, &stmt, 0); 

  // Execute the SQL statement
  sqlite3_bind_text(stmt, 1, u->first_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, u->last_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, std::to_string(u->age).c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, std::to_string(u->gender).c_str(), -1, SQLITE_STATIC);  
  sqlite3_bind_text(stmt, 5, u->race, -1, SQLITE_STATIC);  
  sqlite3_bind_text(stmt, 6, u->eye_color, -1, SQLITE_STATIC);  
  sqlite3_bind_text(stmt, 7, u->medical_condition, -1, SQLITE_STATIC);  
  sqlite3_bind_text(stmt, 8, u->eye_condition, -1, SQLITE_STATIC);  
  sqlite3_bind_text(stmt, 9, u->user_note, -1, SQLITE_STATIC);  
}
