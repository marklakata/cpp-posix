
#include "impl-mqueue.hh"

#include <unistd.h>

#include <cassert>
#include <chrono>

#include "posix.hh"
#include "debug.hh"

#include <istream>
#include <sstream>
#include <fstream>
using namespace std;


using namespace std::chrono;
using std::string;

namespace posix {
namespace {
    /**
     * @param mqfd  [i]     Message queued descriptor to get attributes for.
     *
     * @return mq_attr structure for the inbound message queued fd
     * @throws posix::errno_exception()
     */
    mq_attr 
    get_attributes(mqd_t mqfd) {
        mq_attr attr;
    
        if (mq_getattr(mqfd, &attr) < 0)
            THROW_ERRNO_EXCEPTION();
        
        return attr;
    }
    
    /**
     */
    timespec 
    microseconds_to_timespec(const duration<int64_t, std::nano>& m) {
        enum { Multiplier = duration_cast<nanoseconds>(duration<int64_t>(1)).count() };
    
        timespec ts;
        ts.tv_sec  = duration_cast<seconds>(m).count();
        ts.tv_nsec = m.count() - (ts.tv_sec * Multiplier);
        ts.tv_sec += time(0x0);
    
        return ts;
    }
} // ns

_mqueue_base::_mqueue_base(const std::string& name)
    : _mqueue_base(name, OPEN_FLAG::RDWR)
{}

_mqueue_base::_mqueue_base(const std::string& name, int open_flags)
    : _M_name(name), _M_queue(-1)
{
    _M_queue = mq_open(name.c_str(), open_flags);
    
    if (_M_queue < 0) 
        THROW_ERRNO_EXCEPTION();
}
std::string slurp(std::ifstream& in) {
    return static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str();
}

std::string slurp(std::string in) {
	std::ifstream mmu(in);
    return slurp(mmu);
}


_mqueue_base::_mqueue_base(long mq_msgsize, const string& name, int open_flags 
                         , long mq_maxmsg, long mq_flags, mode_t mode) 
    : _M_name(name), _M_queue(-1)
{ 
    struct mq_attr attrib;
    attrib.mq_flags   = mq_flags;
    attrib.mq_maxmsg  = mq_maxmsg;
    attrib.mq_msgsize = mq_msgsize;
    attrib.mq_curmsgs = 0;
    
//	cout << "name:" << name << endl;
//	cout << "flag:" << hex << open_flags << endl;
//	cout << "attrib.flags " << attrib.mq_flags << endl;
//	cout << "attrib.maxmsg " << attrib.mq_maxmsg << endl;
//	cout << "attrib.msgsize " << attrib.mq_msgsize << endl;
//	cout << "attrib.curmsgs " << attrib.mq_curmsgs << endl;
//	cout << "mode:" << oct << mode << endl;
    _M_queue = mq_open(name.c_str(), OPEN_FLAG::CREAT | open_flags, mode, &attrib);
    
    if (_M_queue < 0) 
	{
		std::string s = slurp("/proc/sys/fs/mqueue/msg_max");
		int32_t msg_max = atoi(s.c_str());
		if (attrib.mq_maxmsg > msg_max) 
		{
			throw runtime_error("Requested Max Messages exceeds system limit (/proc/sys/fs/mqueue/msg_max)");
		}
		
        THROW_ERRNO_EXCEPTION();
	}
}

_mqueue_base::~_mqueue_base() {
    if (_M_queue >= 0)
        mq_close(_M_queue);
}
    
_mqueue_base::_mqueue_base(const _mqueue_base& rhs)
    : _M_name(rhs._M_name)
    , _M_queue(dup(rhs._M_queue))
{}

_mqueue_base::_mqueue_base(_mqueue_base&& rhs)
    : _M_name(rhs._M_name)
    , _M_queue(rhs._M_queue)
{
    rhs._M_queue = -1;
}
    
_mqueue_base&
_mqueue_base::operator=(const _mqueue_base& rhs) {
    if (this == &rhs)
        return *this;
    
    if (_M_queue > 0)
        ::close(_M_queue);
    
    string* name = const_cast<string*>(&this->_M_name);
    *name = rhs._M_name;
    _M_queue = rhs._M_queue > 0 ? dup(rhs._M_queue) : -1;

    return *this;
}

size_t 
_mqueue_base::size() const {
    return get_attributes(_M_queue).mq_curmsgs;
}

size_t
_mqueue_base::capacity() const {
    return get_attributes(_M_queue).mq_maxmsg;
}

void 
_mqueue_base::unlink() { 
    int rc = mq_unlink(_M_name.c_str());

    if (rc < 0)
        THROW_ERRNO_EXCEPTION();
}

void 
_mqueue_base::_M_recv(char* data, size_t len, unsigned* priority) {
    ssize_t rc = mq_receive(_M_queue, data, len, priority);

    if (rc < 0)
        THROW_ERRNO_EXCEPTION();

    assert(static_cast<size_t>(rc) == len);
}
    
void 
_mqueue_base::_M_timed_recv(char* data, size_t len, unsigned* priority
                          , TimerUnits timer) {
    const timespec spec = microseconds_to_timespec(timer);
    
    ssize_t rc = mq_timedreceive(_M_queue, data, len, priority, &spec); 

    if (rc < 0) {
        if (ETIMEDOUT == errno)
            throw timeout_exception();
        else
            THROW_ERRNO_EXCEPTION();
    }

    assert(static_cast<size_t>(rc) == len);
}

void 
_mqueue_base::_M_send(char* data, size_t len, unsigned priority) {
    int rc = mq_send(_M_queue, data, len, priority);

    if (rc < 0)
        THROW_ERRNO_EXCEPTION();

    assert(0 == rc);
}
    
void 
_mqueue_base::_M_timed_send(char* data, size_t len, unsigned priority, TimerUnits timer) {
    const timespec spec = microseconds_to_timespec(timer);
    
    ssize_t rc = mq_timedsend(_M_queue, data, len, priority, &spec); 
    
    if (rc < 0)
        THROW_ERRNO_EXCEPTION();

    assert(0 == rc);
}

} // ns posix
