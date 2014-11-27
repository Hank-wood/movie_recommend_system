#include "package.hpp"

namespace Network {
void send(int fd, const void *buf, std::size_t len) {
    ::send(fd, &len, sizeof(len), 0);
    std::size_t ret = ::send(fd, buf, len, 0);
    assert(ret == len);
}

std::size_t recv(int fd, void *buf) {
    std::size_t len = 0;
    ::recv(fd, &len, sizeof(len), 0);
    std::size_t ret = ::recv(fd, buf, len, MSG_WAITALL);
    assert(ret == len);
    return len;
}

}
