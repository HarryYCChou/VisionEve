/*
 * Copyright (c) 2024 Harry Chou
 */

#ifndef INCLUDE_CAMERA_H_
#define INCLUDE_CAMERA_H_

// ==  c system headers  ==
// == c++ system headers ==
#include <iostream>
#include <string>
// thread
#include <thread>
#include <atomic>
#include <chrono>
// ==   other headers    ==
// spdlog
#include "spdlog/spdlog.h"
#include <spdlog/sinks/ostream_sink.h>
// opencv
#include <opencv2/opencv.hpp>

// using
using std::thread;
using cv::VideoCapture;
using cv::Mat;

class Camera {
 public:
  // constructor
  Camera(int, int, std::shared_ptr<spdlog::logger>);
  ~Camera();

  // log system
  std::shared_ptr<spdlog::logger> logger;

  // function
  void run();
  void stop();
  bool get_image(int, Mat&);
  void set_log(std::string);

 private:
  // log
  std::string log;

  // VideoCapture
  int camera_n_l= 0;
  int camera_n_r= 0;
  bool isOpened_l = false;
  bool isOpened_r = false;
  VideoCapture cap_l;
  VideoCapture cap_r;
  int buffer_size = 10;
  Mat *image_buffer_l;
  Mat *image_buffer_r;
  int buffer_index_l = 0;
  int buffer_index_r = 0;
  void capture();

  // thread
  thread cam_thread;
  std::atomic<bool> isRunning;

  // worker thread
  void workerThread();
};

#endif  // INCLUDE_CAMERA_H_
