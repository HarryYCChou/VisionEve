#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
// thread
#include <thread>
#include <atomic>
#include <chrono>
// socket
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// port
#define PORT 62453

class Server {
public:
  // constructor
  Server();
  ~Server();

  // function
  void run();
  void stop();

private:
  // socket
  int m_server_socket;
  int m_client_socket;
  struct sockaddr_in m_server_address;
  struct sockaddr_in m_client_address;

  // thread
  std::thread thread;
  std::atomic<bool> isRunning;

  // worker thread
  void workerThread();
};
