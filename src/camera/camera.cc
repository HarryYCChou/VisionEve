/*
 * Copyright (c) 2024 Harry Chou
 */

#include "camera/camera.h"

Camera::Camera(int n, std::shared_ptr<spdlog::logger> l) {
  // logger
  logger = l;

  // initializing
  logger->info("Camera[{}] initializing.", n);

  // camera number
  logger->info("Create camera[{}] buffer.", n);
  image_buffer = new Mat[buffer_size];
  logger->info("Open camera[{}]", n);
  camera_n = n;
  cap = cv::VideoCapture(camera_n);
  if (cap.isOpened()) {
    logger->info("Open camera[{}] successfully.", n);
    isOpened = true;
  } else {
    logger->warn("Can't open camera[{}].", n);
  }
}

Camera::~Camera() {
  logger->info("Close camera[{}].", camera_n);
  if (isRunning.load()) {
    stop();
  }
  cap.release();
}

void Camera::run() {
  if (!isOpened) {
    logger->warn("Camera[{}] can't run because device is not opened.", camera_n);
    return;
  }
  if (!isRunning.load()) {
    isRunning.store(true);
    cam_thread = thread([this] {workerThread();});
    logger->info("Camera[{}] thread is running.", camera_n);
  } else {
    logger->warn("Camera[{}] thread is already run.", camera_n);
  }
}

void Camera::stop() {
  if (isRunning.load()) {
    isRunning.store(false);
    if (cam_thread.joinable()) {
      cam_thread.join();
    }
    logger->info("Camera[{}] thread is stopped.", camera_n);
  } else {
    logger->warn("Camera[{}] thread is already stopped.", camera_n);
  }
}

void Camera::workerThread() {
  while (isRunning.load()) {
    capture();
  }
}

void Camera::capture() {
  if (isOpened) {
    cap >> image_buffer[buffer_index];
    if (image_buffer[buffer_index].empty()) {
      isOpened = false;
      logger->warn("Camera[{}] - capture failed", camera_n);
      return;
    }
    buffer_index = (buffer_index + 1) % buffer_size;
  }
}

bool Camera::get_image(Mat& image) {
  if (isOpened && isRunning.load()) {
    image = image_buffer[(buffer_index + buffer_size - 1) % (buffer_size)];
    return true;
  }
  return false;
}

void Camera::set_log(std::string l) {
  log = l;
}
