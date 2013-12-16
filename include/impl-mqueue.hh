
#ifndef __IMPL_QUEUE_HH_
#define __IMPL_QUEUE_HH_

#include <chrono>
#include <string>
#include <mqueue.h>


namespace posix {

class _mqueue_base {
public:
    size_t capacity() const;
    size_t size() const;
    void unlink();

protected:
    /// @def TimerUnits nanosecond level durations 
    using TimerUnits = std::chrono::duration<int64_t, std::nano>;
    
    ///{@
    /** 
     * constructor initializes the internal POSIX mqueue file descriptor and initializes
     * the open_flags to RDWR 
     * @throw errno_exception
     */
    _mqueue_base(const std::string& name);

    /**
     * @see _mqueue_base(const string&)
     * @throw errno_exception
     */
    _mqueue_base(const std::string& name, int open_flags);

    /**
     * Constructor used to create a new POSIX message queue.  Note that if the message
     * queue already exists it will /not/ overwrite the settings (like mq_msgsize,
     * mq_maxmsg).
     *
     * @param mq_msgsize    
     * @param name          
     * @param open_flags    
     * @param mq_maxmsg     
     * @param mq_flags      
     * @param mode
     *
     * @throw errno_exception
     */
    _mqueue_base(long mq_msgsize, const std::string& name, int open_flags, long mq_maxmsg
               , long mq_flags, mode_t mode);
    ///@}
    virtual ~_mqueue_base();
    
    ///{@
    /**
     * 
     */
    _mqueue_base(const _mqueue_base&  rhs);
    _mqueue_base(_mqueue_base&& rhs);
    _mqueue_base& operator=(const _mqueue_base& rhs);
    ///@}
    
    ///{@
    void _M_send(char* data, size_t len, unsigned priority = 0);

    template <typename _TimeSpec>
    void 
    _M_send(char* data, size_t len, unsigned priority
          , std::chrono::duration<TimerUnits::rep , _TimeSpec> timer) {
        _M_timed_send(data
                    , len
                    , priority
                    , std::chrono::duration_cast<std::chrono::nanoseconds>(timer));
    }
    ///@}
    
    ///{@
    void _M_recv(char* data, size_t len, unsigned* priority);

    template <typename _TimeSpec>
    void 
    _M_recv(char* data, size_t len, unsigned* priority 
          , std::chrono::duration<TimerUnits::rep , _TimeSpec> timer) {
        _M_timed_recv(data
                    , len
                    , priority
                    , std::chrono::duration_cast<std::chrono::nanoseconds>(timer));
    }
    ///@}
    
private:
    const std::string _M_name;
    mqd_t _M_queue;
    
    void _M_timed_recv(char* data, size_t len, unsigned* priority, TimerUnits timer);
    void _M_timed_send(char* data, size_t len, unsigned priority, TimerUnits timer);
};
    
} // ns posix

#endif // __IMPL_QUEUE_HH_
