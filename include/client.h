/*
 * Copyright (c) 2023 Harry Chou
 */

#ifndef INCLUDE_CLIENT_H_
#define INCLUDE_CLIENT_H_

// ==  c system headers  ==
#include <unistd.h>
#include <fcntl.h>
// glfw
#include <GLFW/glfw3.h>
// socket
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
// == c++ system headers ==
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
// thread
#include <thread>
#include <atomic>
#include <chrono>
// ==   other headers    ==
// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
// spdlog
#include "spdlog/spdlog.h"
#include <spdlog/sinks/ostream_sink.h>

// port
#define PORT 62453

class Client {
 public:
  // constructor
  Client(std::shared_ptr<spdlog::logger>);
  ~Client();

  // function
  void run();
  void stop();
  void connect_to_server();
  void disconnect_to_server();

 private:
  // logger
  std::shared_ptr<spdlog::logger> logger_;

  // socket
  int m_server_socket;
  int m_client_socket;
  struct sockaddr_in m_server_address;
  struct sockaddr_in m_client_address;
  void send_cmd();

  // thread
  std::thread thread;
  std::atomic<bool> isRunning;

  // UI
  void render();

  // worker thread
  void workerThread();
};

#endif  // INCLUDE_CLIENT_H_
