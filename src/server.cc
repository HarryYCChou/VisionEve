#include <server.h>

Server::Server() : isRunning(false) {
  // initialize for socket 
  m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_server_socket == -1) {
      perror("Server socket creation failed");
      exit(1);
  }

  // define server's address and port to bind to
  struct sockaddr_in m_server_address;
  m_server_address.sin_family = AF_INET;
  m_server_address.sin_port = htons(PORT); // Use your desired port number
  m_server_address.sin_addr.s_addr = INADDR_ANY;

  // bind the socket to the server address
  if (bind(m_server_socket, (struct sockaddr*)&m_server_address, sizeof(m_server_address)) == -1) {
      perror("Server binding failed");
      exit(1);
  }

  // listen for incoming connections
  if (listen(m_server_socket, 5) == -1) {
      perror("Server listening failed");
      exit(1);
  }

}

Server::~Server() {

}

void Server::run() {
  std::cout << "server is running" << std::endl;
  if (!isRunning.load()) {
    isRunning.store(true);
    thread = std::thread([this] { workerThread(); });
  }
}

void Server::stop() {
  if (isRunning.load()) {
    isRunning.store(false);
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void Server::workerThread() {
  while(isRunning.load()) {
    std::cout << "Working in the thread..." << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    // waiting
    std::cout << "Server is waiting for a client to connect..." << std::endl;
  
    // accept incoming connections
    int client_addr_size = sizeof(m_client_address);
    m_client_socket = accept(m_server_socket, (struct sockaddr*)&m_client_address, (socklen_t*)&client_addr_size);
    if (m_client_socket == -1) {
        perror("Server accept failed");
        exit(1);
    }
  
    std::cout << "Client connected." << std::endl;
    // set the socket to non-blocking mode
    //int flags = fcntl(m_client_socket, F_GETFL, 0);
    //if (flags == -1) {
    //    std::cerr << "fcntl F_GETFL failed." << std::endl;
    //    close(m_client_socket);
    //    //return 1;
    //}
    //if (fcntl(m_client_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
    //    std::cerr << "fcntl F_SETFL failed." << std::endl;
    //    close(m_client_socket);
    //    //return 1;
    //}

    // Receive and process data from the connected client
    char buffer[1024] = {0};
    int bytesRead;
    while(isRunning.load()) {
      bytesRead = recv(m_client_socket, buffer, sizeof(buffer), 0);
      if (bytesRead>0) {
        std::cout << "Client sent: " << buffer << std::endl;
        // Clear the buffer for the next data
        memset(buffer, 0, sizeof(buffer));
      }
      //std::cout << "is run" << std::endl;
    }
    //while ((bytesRead = recv(m_client_socket, buffer, sizeof(buffer), 0)) > 0) {
    //    std::cout << "Client sent: " << buffer << std::endl;
    //    // Clear the buffer for the next data
    //    memset(buffer, 0, sizeof(buffer));
    //}
  
    if (bytesRead == 0) {
        std::cout << "Client disconnected." << std::endl;
    } else {
        perror("Server receive failed");
    }
  
    // Close the client socket
    close(m_client_socket);
  }
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

