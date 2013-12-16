
#include <cassert>
#include <chrono>
#include <cstdint>
#include "posix.hh"
#include "debug.hh"
#include "mqueue.hh"

using namespace posix;

void test(mqueue<unsigned> Q) {
    DEBUG("capacity = " << Q.capacity());
    DEBUG("size     = " << Q.size());

    for (size_t i = 0; i < 10000000; ++i)
        Q.recv(std::chrono::duration<int64_t>(100)).second;//== i;
}

int main () {
    mqueue<unsigned> Q("/tqueue", OPEN_FLAG::RDONLY);
    test(Q);
}
