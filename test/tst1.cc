
#include <cassert>
#include <chrono>
#include <cstdint>
#include "posix.hh"
#include "debug.hh"
#include "mqueue.hh"

using namespace posix;
using namespace std;

void test(mqueue<unsigned> Q) {
    DEBUG("capacity = " << Q.capacity());
    DEBUG("size     = " << Q.size());

    for (size_t i = 0; i < 10000000; ++i)
        Q.recv(std::chrono::duration<int64_t>(100)).second;//== i;
}

int main () {
	try 
	{
		cout << "opening queue"<< endl;
		mqueue<unsigned> Q("/tqueue", OPEN_FLAG::RDONLY);
		cout << "receiving 10M messages ... (please wait)" << endl;
		test(Q);
	}
	catch(std::runtime_error& e)
	{
	   cout << "Exception caught: " << e.what() << "\n";
	}
}
