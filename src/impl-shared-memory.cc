
#include "impl-shared-memory.hh"

#include <sys/mman.h>
#include <unistd.h>

#include "posix.hh" 
#include "debug.hh"


using std::string;


namespace posix {
namespace { 
    /**
     */
    void*
    map_fd_to_memory(const size_t& size, int fd, int prot, int flags) {
        void* ptr = ::mmap(0x0, size, prot, flags, fd, 0);

        if (0x0 == ptr)
            throw errno_exception();

        return ptr;
    }

} // ns 

_shared_memory_base::_shared_memory_base(const size_t& size, const string& name)
    : _shared_memory_base(size, name, false)
{ }

_shared_memory_base::~_shared_memory_base() {
    if (0x0 != _M_shm_ptr)
        ::munmap(_M_shm_ptr, _M_size);

    if (_M_shm_fd < 0)
        ::close(_M_shm_fd);
}
    
_shared_memory_base::_shared_memory_base(const size_t& size, const string& name, bool create)
    : _M_size(size), _M_shm_fd(-1), _M_shm_ptr(0x0)
{
    const int oflag = OPEN_FLAG::RDWR | (create ? OPEN_FLAG::CREAT : 0);

    _M_shm_fd = ::shm_open(name.c_str(), oflag, DefaultMode);

    if (create) {
        if (::ftruncate(_M_shm_fd, _M_size) < 0)
            throw errno_exception();
    }

    if (_M_shm_fd < 0)
        throw errno_exception();

    _M_shm_ptr = map_fd_to_memory(_M_size, _M_shm_fd, PROT::READ | PROT::WRITE, oflag);
}

_shared_memory_base::_shared_memory_base(_shared_memory_base&& rhs)
    : _M_size(rhs._M_size), _M_shm_fd(rhs._M_shm_fd), _M_shm_ptr(rhs._M_shm_ptr)
{
    rhs._M_shm_fd  = -1;
    rhs._M_shm_ptr = 0x0;
}

_shared_memory_base::_shared_memory_base(const _shared_memory_base& rhs)
    : _M_size(rhs._M_size), _M_shm_fd(-1), _M_shm_ptr(0x0)
{
    if (rhs._M_shm_fd < 0)
        return;
    else 
        this->operator=(rhs);
}

_shared_memory_base&
_shared_memory_base::operator=(const _shared_memory_base& rhs) {
    
    *(const_cast<size_t*>(&_M_size)) = rhs._M_size;

    if (rhs._M_shm_fd < 0)
        return *this;

    _M_shm_fd = dup(rhs._M_shm_fd);

    if (_M_shm_fd < 0)
        throw errno_exception();

    _M_shm_ptr = map_fd_to_memory(_M_size, _M_shm_fd, PROT::READ | PROT::WRITE 
                                , OPEN_FLAG::RDWR);

    return *this;
}


} // ns posix 
