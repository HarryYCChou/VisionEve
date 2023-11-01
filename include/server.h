#include <iostream>
#include <cstring>
#include <unistd.h>
// thread
#include <thread>
#include <atomic>
#include <chrono>
// socket
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Server {
public:
  // constructor
  Server() : isRunning(false) {}

  // function
  void run();
  void stop();

private:
  std::thread thread;
  std::atomic<bool> isRunning;

  void workerThread();
};
