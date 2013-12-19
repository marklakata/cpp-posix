
#include "impl-memory-map.hh"

#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>


using std::string;


namespace posix {
namespace {
    

} // ns


_memory_map::_memory_map(const string& file_path) 
    : _M_file_path(file_path), _M_offset(0), _M_file_size(0), _M_permission(-1)
    , _M_scope(-1), _M_fd(-1), _M_ptr(0x0)
{ }

_memory_map::~_memory_map() {
    this->close();
}

size_t 
_memory_map::open(int permission, int scope, int offset) {
    assert(0x0 == _M_ptr);
    assert(-1  == _M_fd );
}

void 
_memory_map::close() {
    if (_M_ptr) {
        ::munmap(_M_ptr, _M_file_size);
        _M_ptr = 0x0;
    }

    if (_M_fd > 0) {
        ::close(_M_fd);
        _M_fd = - 1;
    }
}

bool 
_memory_map::_M_truncated(const size_t& block_size) const {
    assert(static_cast<bool>(*this));
    return 0 == _M_file_size % block_size;
}

} // ns posix 
