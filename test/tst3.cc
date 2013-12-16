

#include "debug.hh"
#include "posix.hh"
#include "shared-memory.hh"

using namespace posix;


int main () {
    shared_memory<double> data("here", true);
    
    DEBUG(*data);
    *data = 3.2;
    DEBUG(*data);
}
