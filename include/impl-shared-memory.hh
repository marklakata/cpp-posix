
#ifndef __IMPL_SHARED_MEMORY_HH_
#define __IMPL_SHARED_MEMORY_HH_ 

#include <string>
#include "posix.hh"


namespace posix {

class _shared_memory_base {
public:
    _shared_memory_base(const size_t& size, const std::string& name);
    _shared_memory_base(const size_t& size, const std::string& name, bool create);
    virtual ~_shared_memory_base();

    ///{@
    _shared_memory_base(_shared_memory_base&& rhs);
    _shared_memory_base(const _shared_memory_base& rhs);
    _shared_memory_base& operator=(const _shared_memory_base& rhs);
    ///@} 
    
protected:
    template <typename _Tp>
    inline _Tp*
    data() {
        return static_cast<_Tp*>(_M_shm_ptr);
    }

private:
    enum { DefaultMode = SYM::IRUSR | SYM::IWUSR };

    const size_t _M_size;
    int _M_shm_fd;
    void* _M_shm_ptr;
};

} // ns posix 

#endif // __SHARED_MEMORY_HH_
