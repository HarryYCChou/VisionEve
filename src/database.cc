/*
 * Copyright (c) 2023 Harry Chou
 */

#include "./database.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

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

  if(rc!=SQLITE_OK) {
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
  //std::string sql_insert_data = "INSERT INTO patients (first_name, last_name, age, gender, race, eye_color, medical_condition, eye_condition, user_note) VALUES ('a', 'a', 0, 0, '', 'a', 'a', 'a', 'a')";
  //std::string sql_insert_data = "INSERT INTO patients VALUES (2, 'a', 'a', 0, 0, '', 'a', 'a', 'a', 'a');";
  char *sql_insert_data;
  //sql_insert_data = "INSERT INTO patients VALUES (2, 'a', 'a', 0, 0, 'a', 'a', 'a', 'a', 'a');";
  sql_insert_data = "SELECT * FROM patients;";

  //db.execute(sql_insert_data.c_str(), callback);
  if (db!=nullptr) {

  //int rc = sqlite3_exec(db, sql_insert_data.c_str(), callback, 0, &zErrMsg);
  //int rc = sqlite3_exec(db, sql_insert_data, NULL, NULL, &zErrMsg);
  int rc = sqlite3_exec(db, ";", NULL, NULL, &zErrMsg);
  }
  //return;
  //const char* sql_insert_data =
  //  "INSERT INTO employees (id, first_name, last_name, age, gender, race, eye_color, medical_condition, eye_condition, user_note) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

  //// Prepare the SQL statement
  //sqlite3_stmt* stmt;
  //int rc = sqlite3_prepare_v2(db, sql_insert_data, -1, &stmt, 0); 
  //return;

  //// Execute the SQL statement
  //sqlite3_bind_text(stmt, 1, u->first_name, -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 2, u->last_name, -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 3, std::to_string(u->age).c_str(), -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 4, std::to_string(u->gender).c_str(), -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 5, u->race, -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 6, u->eye_color, -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 7, u->medical_condition, -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 8, u->eye_condition, -1, SQLITE_STATIC);
  //sqlite3_bind_text(stmt, 9, u->user_note, -1, SQLITE_STATIC);
}
