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
    new_data.age = std::stoi(argv[3]);
    strcpy(new_data.gender, argv[4]);
    strcpy(new_data.race, argv[5]);
    strcpy(new_data.eye_color, argv[6]);
    strcpy(new_data.eye_condition, argv[7]);
    strcpy(new_data.user_note, argv[8]);

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
  char *zErrMsg = 0;
  int rc;

  std::string sql_insert_data = std::string("INSERT INTO PATIENTS ") +
"(FIRST_NAME, LAST_NAME, AGE, GENDER, RACE, EYE_COLOR, EYE_CONDITION, USER_NOTES) " +"VALUES ('', '', 0, '', '', '', '', '');";

  rc = sqlite3_exec(db, sql_insert_data.c_str(), 0, 0, &zErrMsg);

  if(rc!=SQLITE_OK) {
    logger->error("function add_user error");
    return;
  } else {
    logger->info("add_user successfully");
  }
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

void Database::update_user(User* u) {
  char *zErrMsg = 0;
  int rc;

  //std::string sql_insert_data = std::string("INSERT INTO PATIENTS ") +
//"(FIRST_NAME, LAST_NAME, AGE, GENDER, RACE, EYE_COLOR, MEDICAL_CONDITION, EYE_CONDITION, USER_NOTES) " +"VALUES ('New', 'User', 0, 0, '', '', '', '', '');";
  
  std::string sql_insert_data = std::string("UPDATE PATIENTS ") +
  std::string("SET FIRST_NAME = '") + u->first_name + std::string("',") +
  std::string("LAST_NAME = '") + u->last_name + std::string("',") +
  std::string("AGE = '") + std::to_string(u->age) + std::string("',") +
  std::string("GENDER = '") + u->gender + std::string("',") +
  std::string("RACE = '") + u->race + std::string("',") +
  std::string("EYE_COLOR = '") + u->eye_color + std::string("',") +
  std::string("EYE_CONDITION = '") + u->eye_condition + std::string("',") +
  std::string("USER_NOTES = '") + u->user_note + std::string("'") +
  std::string("WHERE ID = ") + std::to_string(u->id) + std::string(";");

//, LAST_NAME, AGE, GENDER, RACE, EYE_COLOR, MEDICAL_CONDITION, EYE_CONDITION, USER_NOTES) " +"VALUES ('New', 'User', 0, 0, '', '', '', '', '');";

  rc = sqlite3_exec(db, sql_insert_data.c_str(), 0, 0, &zErrMsg);

  if(rc!=SQLITE_OK) {
    logger->error("function update_user error");
    return;
  } else {
    logger->info("update_user successfully");
  }
}

std::vector<User> Database::get_user() {
  char *zErrMsg = 0;
  int rc;
  std::vector<User> user_data;

  std::string sql_query = "SELECT ID, FIRST_NAME, LAST_NAME, AGE, GENDER, RACE, EYE_COLOR, EYE_CONDITION, USER_NOTES FROM patients;";

  rc = sqlite3_exec(db, sql_query.c_str(), get_user_cb, &user_data, &zErrMsg);

  if(rc!=SQLITE_OK) {
    logger->error("function get_user error");
    return user_data;
  } else {
    logger->info("get_user successfully");
  }

  return user_data;  
}
