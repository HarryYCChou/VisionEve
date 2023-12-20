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

static int get_user_cb(void* data, int argc, char** argv, char** colNames) {
    std::vector<User>* users = reinterpret_cast<std::vector<User>*>(data);

    User new_data;
    new_data.id = std::stoi(argv[0]);
    strcpy(new_data.first_name, argv[1]);
    strcpy(new_data.last_name, argv[2]);
    //newData.name = argv[1];
    //newData.value = std::stod(argv[2]);

    users->push_back(new_data);

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

void Database::del_user(int id) {
  char *zErrMsg = 0;
  int rc;

  std::string sql_query = "DELETE FROM " +
                          table_name + " " +
                          "WHERE ID = " + std::to_string(id) +
                          ";";

  rc = sqlite3_exec(db, sql_query.c_str(), NULL, NULL, &zErrMsg);

  if(rc!=SQLITE_OK) {
    logger->error("function del_user error");
    return;
  } else {
    logger->info("del_user successfully");
  }
}

std::vector<User> Database::get_user() {
  char *zErrMsg = 0;
  int rc;
  std::vector<User> user_data;

  std::string sql_query = "SELECT ID, FIRST_NAME, LAST_NAME, AGE, GENDER, RACE, EYE_COLOR, MEDICAL_CONDITION, EYE_CONDITION, USER_NOTES FROM patients;";

  rc = sqlite3_exec(db, sql_query.c_str(), get_user_cb, &user_data, &zErrMsg);

  if(rc!=SQLITE_OK) {
    logger->error("function get_user error");
    return user_data;
  } else {
    logger->info("get_user successfully");
  }

  return user_data;  
}
