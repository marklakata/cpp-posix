
#ifndef __POSIX_HH_
#define __POSIX_HH_ 

#include <sys/mman.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>


//////////////////////////////////////////////////////////////////////////////////////////
// USEFUL GLOBAL MACROS                                                                 // 
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>

#define DEFINE_EXCEPTION(_name_, _parent_) \
    class _name_ : public _parent_ { \
    public: \
        _name_(const std::string& msg) \
            : _parent_(msg) { \
            DEBUG(msg); \
        } \
    };


namespace posix {

//////////////////////////////////////////////////////////////////////////////////////////
// POSIX ENUMERATIONS                                                                   // 
//////////////////////////////////////////////////////////////////////////////////////////

namespace OPEN_FLAG {
enum {
    RDONLY      = O_RDONLY
  , WRONLY      = O_WRONLY
  , RDWR        = O_RDWR
  , NONBLOCK    = O_NONBLOCK
  , CREAT       = O_CREAT
  , EXCL        = O_EXCL
};
} // OPEN_FLAG 

namespace ERRNO {
enum {
    ACCES       = EACCES 
};
} // ERRNO 

namespace MAP {
enum {
    SHARED  = MAP_SHARED 
  , PRIVATE = MAP_PRIVATE
  , FIXED   = MAP_FIXED 
};
} // MAP  

namespace PROT { 
enum {
    READ    = PROT_READ
  , WRITE   = PROT_WRITE
  , EXEC    = PROT_EXEC
  , NONE    = PROT_NONE
};
} // PROTO

namespace SYM {
enum {
    IRUSR   = S_IRUSR
  , IWUSR   = S_IWUSR
  , IXUSR   = S_IXUSR
  , IRWXU   = S_IRWXU
  , IRGRP   = S_IRGRP
  , IWGRP   = S_IWGRP
  , IXGRP   = S_IXGRP
  , IRWXG   = S_IRWXG
  , IROTH   = S_IROTH
  , IWOTH   = S_IWOTH
  , IXOTH   = S_IXOTH
  , IRWXO   = S_IRWXO	
};
} // SYM

//////////////////////////////////////////////////////////////////////////////////////////
// POSIX EXCEPTIONS                                                                     //
//////////////////////////////////////////////////////////////////////////////////////////

class errno_exception : public std::runtime_error {
public:
    errno_exception()
        : errno_exception(errno)
    {}

    errno_exception(int errno_)
        : errno_exception(strerror(errno_))
    {}

    errno_exception(const std::string& message)
        : std::runtime_error(message), errorno(errno)
    {}

    const int errorno;
};


/**
 */
class timeout_exception : public errno_exception {
public:
    timeout_exception()
        : errno_exception()
    {}
};

} // ns posix 

#endif // __POSIX_HH_
