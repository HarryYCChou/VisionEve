/*
 * Copyright (c) 2023 Harry Chou
 */

#include "./database.h"

Database::Database(std::shared_ptr<spdlog::logger> lg) {
  // setup logger
  logger = lg;

  logger->info("Database initializing.");
}

