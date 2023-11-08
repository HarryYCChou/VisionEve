#include "./client.h"

int main() {
  // create client socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
      perror("Client socket creation failed");
      exit(1);
  }

  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(62453); // Use the same port as the server
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address

  // connect to server
  if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
      perror("Client connection failed");
      exit(1);
  }

  // sending data
  //const char* message = "Hello, Server!";
  //send(clientSocket, message, strlen(message), 0);
  while(1) {
    std::cout << "Please enter a phrase or [q] to exit: ";
    std::string strInput;
    std::cin >> strInput;

    if (strInput.compare("q")==0) break;

    // convert the std::string to a char*
    char* msg = new char[strInput.length()+1];  // +1 for the null terminator

    // copy
    std::strcpy(msg, strInput.c_str());
    send(clientSocket, msg, strlen(msg), 0);
  }

  // close socket
  close(clientSocket);

  return 0;
}

