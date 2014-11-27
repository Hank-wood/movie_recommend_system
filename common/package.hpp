#ifndef _PACKAGE_HPP_
#define _PACKAGE_HPP_

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <assert.h>

namespace Network {
struct package {
    enum { FETCH, COMMIT } tag;
};

void send(int fd, const void *buf, std::size_t len);
std::size_t recv(int fd, void *buf);

template<typename T>
void send(int fd, const T& data) {
    send(fd, &data, sizeof(data));
}
}
#endif
