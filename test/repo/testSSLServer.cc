#include "repo/ssl/SSLSocket.h"

int main(int argc, char *argv[]) {
  const char *portnum = argc < 2 ? "8000" : argv[1];
  SSLSocket s = SSLSocket::createServer(portnum);
}
