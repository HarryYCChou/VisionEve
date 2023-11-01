#include <server.h>

void Server::run() {
  std::cout << "server is running" << std::endl;
}

void Server::stop() {

}

void Server::workerThread() {

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

