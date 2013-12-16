
#ifndef __SHARED_MEMORY_HH_
#define __SHARED_MEMORY_HH_

#include <string>

#include "impl-shared-memory.hh"


namespace posix {

/**
 * @class       shared_memory
 * @template    typename _Tp
 *
 * @remark reentrant
 * @remark threadsafe
 */
template <typename _Tp>
class shared_memory : public _shared_memory_base {

    static_assert(std::is_pod<_Tp>::value, "Template type is non POD type");

public:
    typedef shared_memory<_Tp> _Self;
    typedef _Tp                value_type;
    typedef _Tp*               pointer;
    typedef _Tp&               reference;
    typedef const _Tp&         const_reference;


    template <typename... _Args>
    shared_memory(_Args&&... args)
        : _shared_memory_base(sizeof(_Tp), std::forward<_Args>(args)...)
    {}

    /**
     * @return mutable reference to the shared data member as a ref
     */
    reference
    operator*() {
        return *this->data<_Tp>();
    }

    /**
     * @return mutable reference to the shared data member as a pointer
     */
    pointer
    operator->() {
        return this->data<_Tp>();
    }

    /**
     */
    reference
    operator[](const size_t& index) {
        return this->data<_Tp>()[index];
    }

    /** const correct version of shared_memory::operator[] */
    const_reference
    operator[](const size_t& index) const {
        return this->data<_Tp>()[index];
    }
};

} // ns posix

#endif // __SHARED_MEMORY_HH_
