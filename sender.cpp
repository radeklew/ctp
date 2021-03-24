// This is the program that sends data.

#include <iostream>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Test string
const std::string DATA = "Hello, world!";

int main(int argc, char** argv){

  // Ideally, there should be two arguments, plus the name of the program.
  if (argc < 3){
    std::cout << "Not enough arguments" << std::endl;
    return 1;
  }
  
  // If there are more than two arguments, we'll just
  // assume the rest are garbage.
  else if (argc > 3){
    std::cout
      << "That's a lot of arguments. Using the first two." << std::endl;
  }

  // Name the arguments.
  std::string hostname = argv[1];
  std::string portname = argv[2];

  // Set hints for struct creation
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints)); // This is cursed?
  hints.ai_family    = AF_INET;
  hints.ai_socktype  = SOCK_DGRAM;
  hints.ai_protocol  = 0;
  hints.ai_flags     = AI_ADDRCONFIG;
  struct addrinfo* res=0;
  
  // Create a struct for the remote host.
  int err = getaddrinfo(hostname.c_str(), portname.c_str(), &hints, &res);
  //add error catch here

  // Create the client socket
  int fd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  //add error catch here

  // SEND it
  sendto(fd, DATA.c_str(), sizeof(DATA.c_str()), 0,
	 res->ai_addr, res->ai_addrlen);

  // Clean up
  freeaddrinfo(res);
  return 0;
}
