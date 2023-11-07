#include <server.h>

Server::Server(std::shared_ptr<spdlog::logger> logger)
    : isRunning(false), logger_(logger) {
  logger_->info("Server initializing.");
  // initialize for socket 
  logger_->info("Socket initializing.");
  m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_server_socket == -1) {
    logger_->error("Server socket creation failed.");
    exit(1);
  }

  // define server's address and port to bind to
  struct sockaddr_in m_server_address;
  m_server_address.sin_family = AF_INET;
  m_server_address.sin_port = htons(PORT); // Use your desired port number
  m_server_address.sin_addr.s_addr = INADDR_ANY;

  // bind the socket to the server address
  if (bind(m_server_socket, (struct sockaddr*)&m_server_address, sizeof(m_server_address)) == -1) {
    logger_->error("Server binding failed.");
    exit(1);
  }

  // listen for incoming connections
  if (listen(m_server_socket, 5) == -1) {
    logger_->error("Server listening failed.");
    exit(1);
  }

  // set the server socket to non-blocking mode
  int flags = fcntl(m_server_socket, F_GETFL, 0);
  if (flags == -1) {
    logger_->error("fcntl F_GETFL failed.");
    close(m_server_socket);
  }
  if (fcntl(m_server_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
    logger_->error("fcntl F_SETFL failed.");
    close(m_server_socket);
  }
  logger_->info("Server initialization completed.");
}

Server::~Server() {

}

void Server::run() {
  if (!isRunning.load()) {
    isRunning.store(true);
    thread = std::thread([this] { workerThread(); });
    logger_->info("Server is running.");
  } else {
    logger_->warn("Server is already run.");
  }
}

void Server::stop() {
  if (isRunning.load()) {
    isRunning.store(false);
    if (thread.joinable()) {
      thread.join();
    }
    logger_->info("Server is stopped.");
  } else {
    logger_->warn("Server is already stopped.");
  }
}

void Server::render() {

}

void Server::workerThread() {
  while(isRunning.load()) {
    // set client socket to -1
    m_client_socket = -1;
    // set timeout
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(m_server_socket, &readfds);


    logger_->debug("Socket select");
    // Wait for an event on the server socket
    int ret = select(m_server_socket + 1, &readfds, NULL, NULL, &timeout);
    if (ret < 0) {
        std::cerr << "Select failed." << std::endl;
        break;
    } else if ( ret == 0 ) {
      // timeout
      logger_->debug("Socket select timeout.");
      continue;
    }

    logger_->info("Wait for client accept");
    if (FD_ISSET(m_server_socket, &readfds)) {
      // Accept a client connection (assuming only one client)
      if (m_client_socket == -1) {
          m_client_socket = accept(m_server_socket, NULL, NULL);
          if (m_client_socket != -1) {
              // Set the client socket to non-blocking mode
              int m_client_socketFlags = fcntl(m_client_socket, F_GETFL, 0);
              if (m_client_socketFlags == -1) {
                  logger_->error("fcntl F_GETFL for client socket failed.");
                  close(m_client_socket);
                  break;
              }
              if (fcntl(m_client_socket, F_SETFL, m_client_socketFlags | O_NONBLOCK) == -1) {
                  logger_->error("fcntl F_SETFL for client socket failed.");
                  close(m_client_socket);
                  break;
              }
              logger_->info("Client connected.");
          }
      }
    }

    if (m_client_socket != -1) {
        // Handle client data (read/write) in non-blocking mode
        // You can use the m_client_socket to receive and send data here
        logger_->debug("Ready for getting data.");
        // Receive and process data from the connected client
        char buffer[1024] = {0};
        int bytesRead;
        while(isRunning.load()) {
          bytesRead = recv(m_client_socket, buffer, sizeof(buffer), 0);
          if (bytesRead>0) {
            logger_->info("Client sent: {}", buffer);
            // Clear the buffer for the next data
            memset(buffer, 0, sizeof(buffer));
          } else if (bytesRead==0) {
            logger_->info("Client disconnected.");
            break;
          }
        }
    }
  }

  // Clean up and close sockets...
  if (m_client_socket != -1) {
      close(m_client_socket);
  }
  close(m_server_socket);

  return;
}

//int main() {
//  // initialize for socket 
//  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//  if (serverSocket == -1) {
//      perror("Server socket creation failed");
//      exit(1);
//  }
//
//  // define server's address and port to bind to
//  struct sockaddr_in serverAddress;
//  serverAddress.sin_family = AF_INET;
//  serverAddress.sin_port = htons(62453); // Use your desired port number
//  serverAddress.sin_addr.s_addr = INADDR_ANY;
//
//  // bind the socket to the server address
//  if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
//      perror("Server binding failed");
//      exit(1);
//  }
//
//  // listen for incoming connections
//  if (listen(serverSocket, 5) == -1) {
//      perror("Server listening failed");
//      exit(1);
//  }
//
//  while (true) {
//    // waiting
//    std::cout << "Server is waiting for a client to connect..." << std::endl;
//
//    // accept incoming connections
//    struct sockaddr_in clientAddress;
//    int clientSocket, clientAddrSize = sizeof(clientAddress);
//    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddrSize);
//    if (clientSocket == -1) {
//        perror("Server accept failed");
//        exit(1);
//    }
//
//      std::cout << "Client connected." << std::endl;
//
//      // Receive and process data from the connected client
//      char buffer[1024] = {0};
//      int bytesRead;
//      while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
//          std::cout << "Client sent: " << buffer << std::endl;
//          // Clear the buffer for the next data
//          memset(buffer, 0, sizeof(buffer));
//      }
//
//      if (bytesRead == 0) {
//          std::cout << "Client disconnected." << std::endl;
//      } else {
//          perror("Server receive failed");
//      }
//
//      // Close the client socket
//      close(clientSocket);        
//
//  }
//
//  return 0;
//}

