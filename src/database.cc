/*
 * Copyright (c) 2023 Harry Chou
 */

#include "./database.h"

//static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
//   int i;
//   for(i = 0; i<argc; i++) {
//      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//   }
//   printf("\n");
//   return 0;
//}

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
  sqlite3_close(db);
}

void Database::add_user(User* u) {
  sqlite3 *database=NULL;
  char *zErrMsg = 0;
  int rc;

  /*FIXME 
    workaround: database should reopen again,
    *db value created in constructor can not be used,
    or it will segmentation fault.
  */
  rc = sqlite3_open("./test.db", &database);

  /* FIXME
     logger has an issue when logger->info called
  */
  //if(rc!=SQLITE_OK) {
  //  logger->warn("Can't open database");
  //  return;
  //} else {
  //  std::cout << "OK3" << std::endl;
  //  logger->info("Opened database successfully");
  //  std::cout << "OK3" << std::endl;
  //}

  std::string sql_insert_data = "INSERT INTO PATIENTS (FIRST_NAME, LAST_NAME, AGE, GENDER, RACE, EYE_COLOR, MEDICAL_CONDITION, EYE_CONDITION, USER_NOTES) VALUES ('a', 'a', 0, 0, 'a', 'a', 'a', 'a', 'a');";
  rc = sqlite3_exec(database, sql_insert_data.c_str(), 0, 0, &zErrMsg);
  sqlite3_close(database);
}
