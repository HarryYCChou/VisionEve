/*
 * Copyright (c) 2024 Harry Chou
 */

#include "camera/camera.h"

Camera::Camera(int l_n, int r_n, std::shared_ptr<spdlog::logger> l) {
  // logger
  logger = l;

  // initializing
  logger->info("Camera[{}] initializing.", l_n);
  logger->info("Camera[{}] initializing.", r_n);

  // camera number
  logger->info("Create camera[{}] buffer.", l_n);
  logger->info("Create camera[{}] buffer.", r_n);
  image_buffer_l = new Mat[buffer_size];
  image_buffer_r = new Mat[buffer_size];

  // open L
  logger->info("Open camera[{}]", l_n);
  camera_n_l = l_n;
  cap_l = cv::VideoCapture(camera_n_l);
  if (cap_l.isOpened()) {
    logger->info("Open camera[{}] successfully.", l_n);
    isOpened_l = true;
  } else {
    logger->warn("Can't open camera[{}].", l_n);
  }
  // open R
  logger->info("Open camera[{}]", r_n);
  camera_n_r = r_n;
  cap_r = cv::VideoCapture(camera_n_r);
  if (cap_r.isOpened()) {
    logger->info("Open camera[{}] successfully.", r_n);
    isOpened_r = true;
  } else {
    logger->warn("Can't open camera[{}].", r_n);
  }
}

Camera::~Camera() {
  logger->info("Close camera[{}].", camera_n_l);
  logger->info("Close camera[{}].", camera_n_r);
  if (isRunning.load()) {
    stop();
  }
  cap_l.release();
  cap_r.release();
}

void Camera::run() {
  if (!isOpened_l) {
    logger->warn("Camera[{}] can't run because device is not opened.", camera_n_l);
  }
  if (!isOpened_r) {
    logger->warn("Camera[{}] can't run because device is not opened.", camera_n_r);
  }
  if (!isRunning.load()) {
    isRunning.store(true);
    cam_thread = thread([this] {workerThread();});
    logger->info("Camera thread is running.");
  } else {
    logger->warn("Camera thread is already run.");
  }
}

void Camera::stop() {
  if (isRunning.load()) {
    isRunning.store(false);
    if (cam_thread.joinable()) {
      cam_thread.join();
    }
    logger->info("Camera thread is stopped.");
  } else {
    logger->warn("Camera thread is already stopped.");
  }
}

void Camera::workerThread() {
  while (isRunning.load()) {
    capture();
  }
}

void Camera::capture() {
  if (isOpened_l) {
    cap_l >> image_buffer_l[buffer_index_l];
    if (image_buffer_l[buffer_index_l].empty()) {
      isOpened_l = false;
      logger->warn("Camera[{}] - capture failed", camera_n_l);
    } else {
      buffer_index_l = (buffer_index_l + 1) % buffer_size;
    }
  }
  if (isOpened_r) {
    cap_r >> image_buffer_r[buffer_index_r];
    if (image_buffer_r[buffer_index_r].empty()) {
      isOpened_r = false;
      logger->warn("Camera[{}] - capture failed", camera_n_r);
    } else {
      buffer_index_r = (buffer_index_r + 1) % buffer_size;
    }
  }
}

bool Camera::get_image(int device, Mat& image) {
  if (device == 0) {
    if (isOpened_l && isRunning.load()) {
      image = image_buffer_l[(buffer_index_l + buffer_size - 1) % (buffer_size)];
      return true;
    }
  } else {
    if (isOpened_r && isRunning.load()) {
      image = image_buffer_r[(buffer_index_r + buffer_size - 1) % (buffer_size)];
      return true;
    }
  }
  return false;
}

void Camera::set_log(std::string l) {
  log = l;
}
