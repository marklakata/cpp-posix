
#ifndef __MQUEUE_HH_
#define __MQUEUE_HH_

#include <string>

#include "impl-mqueue.hh"
#include "posix.hh"


namespace posix {
/**
 * @class       mqueue
 * @template    typename _Tp
 *
 * Acts as a C++ interface to the POSIX mqueue interface.  Its interface functions are
 * reasonably inferable from the C interface caveat the interfaces which normally used
 * timspec values.  The timespec value in the C interface were defining a target point in
 * time whereas the C++ interface uses a std::chrono::duration<int64_t> which defines the
 * number of (arbitrary time units) from now.
 *
 * @remark reentrant
 * @remark threadsafe
 */
template <typename _Tp>
class mqueue : public _mqueue_base {

    static_assert(std::is_pod<_Tp>::value, "Template type is non POD type");

public:
    typedef mqueue<_Tp> _Self;
    typedef _Tp         value_type;
    typedef _Tp*        pointer;
    typedef _Tp&        reference;
    typedef const _Tp&  const_reference;

    /**
     * @param [i, ] name        (string)
     * @param [i, ] open_flags  (int)
     * @param [i, ] mq_maxmsg   (long)
     * @param [i, ] mq_flags    (long)
     * @param [i, ] mode        (mode_t)
     */
    template <typename... _Args>
    static _Self
    create_queue(_Args&&... args) {
        return _Self(sizeof(value_type), std::forward<_Args>(args)...);
    }

    ///@{
    /**
     *
     * -----------------------------------------------------------------------------------
     * @fn  *mqueue::mqueue()
     * @param [i, ] name
     *
     * -----------------------------------------------------------------------------------
     * @fn  *mqueue::mqueue()
     * @param [i, ] name
     * @param [i, ] open_flags
     *
     */
    template <typename... _Args>
    mqueue(_Args&&... args)
        : _mqueue_base(std::forward<_Args>(args)...)
    {}
    ///@}

    /**
     * Sends some item
     * -----------------------------------------------------------------------------------
     * @fn *mqueue::send(_Tp data)
     * @param [i, ] data
     *
     * -----------------------------------------------------------------------------------
     * @fn *mqueue::send(_Tp data, unsigned priority)
     * @param [i, ] data
     * @param [i, ] priority
     *
     * -----------------------------------------------------------------------------------
     * @fn *mqueue::send(_Tp data, unsigned priority)
     * @param [i, ] data
     * @param [i, ] priority
     *
     * -----------------------------------------------------------------------------------
     * @fn *mqueue::send(_Tp data, unsigned priority
     *                 , std::chrono::duration<int64_t, _TimeSpec> timeval)
     *
     * @param [i, ] data
     * @param [i, ] priority
     * @param [i, ] timeval
     */
    template <typename... _Args>
    void
    send(const_reference data,  _Args&&... args) {
        _M_send((char*) &data, sizeof(_Tp), std::forward<_Args>(args)...);
    }

    /**
     *
     * -----------------------------------------------------------------------------------
     * @throws
     * @return
     *      pair<
     *          unsigned    => priority
     *          _Tp         => value
     *          >
     *
     */
    template <typename... _Args>
    std::pair<unsigned, value_type>
    recv(_Args&&... args) {
        value_type data;
        unsigned   priority;

        _M_recv(reinterpret_cast<char*>(&data), sizeof(_Tp), &priority
              , std::forward<_Args>(args)...);

        return std::make_pair(priority, data);
    }
};
} // ns posix

#endif //__MQUEUE_HH_
