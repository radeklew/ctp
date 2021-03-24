// This is the program that receives data.

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>

void handle_datagram(char *buffer, ssize_t n){
  for(ssize_t i = 0; i < n; ++i){
    std::cout << buffer[i];
  }
  std::cout << std::endl;
}

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

  // Construct the hint struct
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints)); //still cursed
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_PASSIVE|AI_ADDRCONFIG;
  struct addrinfo* res = 0;

  // Get socket info
  int err = getaddrinfo(hostname.c_str(), portname.c_str(), &hints, &res);
  //add error check

  // Create socket
  int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  //add error check

  if (bind(fd, res->ai_addr, res->ai_addrlen)){
    //add error check
  }
  freeaddrinfo(res);

  // Create buffers for incoming data.
  char buffer[80];
  struct sockaddr_storage src_addr;
  socklen_t src_addr_len = sizeof(src_addr);

  // Receive.
  ssize_t count = recvfrom(fd, buffer, sizeof(buffer), 0,
			   (struct sockaddr*)&src_addr, &src_addr_len);
  if(count == -1){
    //add error check
  } else if (count == sizeof(buffer)) {
    //add error check
  } else {
    handle_datagram(buffer, count);
  }
  
  return 0;
}
