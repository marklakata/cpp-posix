
#include <cassert>
#include "posix.hh"
#include "debug.hh"
#include "mqueue.hh"

using namespace posix;

int main () {
    try { 
        mqueue<unsigned>("/tqueue").unlink();
    } catch (...) {}

    auto Q = mqueue<unsigned>::create_queue("/tqueue"        /* queue name */
                                          , OPEN_FLAG::RDWR  /* open flags */
                                          , 600              /* max queue size */
                                          , 0                /* mq flags */
                                          , 075);
    
    for (size_t i = 0; i < 10000000; ++i) 
        Q.send(i);
}
