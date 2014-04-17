
#include <cassert>
#include "posix.hh"
#include "debug.hh"
#include "mqueue.hh"
#include <iostream>

using namespace posix;
using namespace std;

int main () {
    try { 
        mqueue<unsigned>("/tqueue").unlink();
    } catch (...) 
	{
	}

	try 
	{
		cout << "creating queue" << endl;
		
		auto Q = mqueue<unsigned>::create_queue("/tqueue"        /* queue name */
											  , OPEN_FLAG::RDWR  /* open flags */
											  , 10               /* max queue size */
											  , 0                /* mq flags */
											  , 0755);
		
		cout << "sending 10M messages ... (please wait)" << endl;
		for (size_t i = 0; i < 10000000; ++i) 
			Q.send(i);
		cout << "sent!" << endl;
	}
	catch(std::runtime_error& e)
	{
	   cout << "Exception caught: " << e.what() << "\n";
	}
}
