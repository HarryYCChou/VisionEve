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
#include <vector>
#include <iomanip>
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
// stb_image
#include "stb/stb_image.h"
// user
#include "./user.h"
// database
#include "./database.h"
// camera
#include "camera/camera.h"

// port
#define PORT 62453

class Client {
 public:
  // constructor
  Client(std::shared_ptr<spdlog::logger>);
  ~Client();

  // log system
  std::stringstream logCaptureStream;
  std::shared_ptr<spdlog::logger> logger;

  // database
  Database *db = nullptr;

  // camera
  Camera *cam = nullptr;
  //Camera *cam_R = nullptr;
  Mat image_buf;

  // user data
  User *user_info = nullptr;
  std::vector<User> all_user;
  void update_all_user_info();

  // function
  void run();
  void stop();
  void connect_to_server();
  void disconnect_to_server();
  void set_log(std::string);

  // UI
  bool exit_requested = false;
  void render();
  void render_all_user();
  void render_patient_data();
  void render_camera_data();
  void render_lbs_control();
  void render_side_panel();

  // camera data
  GLuint textureID_L, textureID_R;
  GLuint textureID_CamL, textureID_CamR;
  void load_texture(GLuint&, Mat&);
  GLuint load_texture(const char*);

  // LBS data
  // FIXME: this section should be seperated to another class
  GLuint textureID_LBS_L, textureID_LBS_R;
  int lbs_brightness = 43;
  int lbs_contrast = 87;
  // LBS content
  void render_lbs_content();

  // font
  ImFont* opensans_reg_font;
  ImFont* opensans_reg_font_s;
  ImFont* opensans_reg_font_l;

 private:
  // log
  std::string log;

  // logo
  GLuint textureID_logo;

  // socket
  int m_server_socket;
  int m_client_socket;
  struct sockaddr_in m_server_address;
  struct sockaddr_in m_client_address;
  void send_cmd();

  // thread
  std::thread thread;
  std::atomic<bool> isRunning;

  // worker thread
  void workerThread();
};

#endif  // INCLUDE_CLIENT_H_
