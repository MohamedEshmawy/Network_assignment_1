#include "timer.h"
#include "event_queue.h"
#include "windows.h"
using namespace std;

set<seq_nr> timer::timers;

void timer::start_timer(seq_nr s, int time)
{
	set<seq_nr>::iterator it;
	timers.insert(s);
	Sleep(time);
	it=timers.find(s);
	if (it != timers.end())
	{
		event_queue::add_event(ack_timeout);
		timers.erase (it);
	}

}

void timer::stop_timer(seq_nr k)
{
    set<seq_nr>::iterator it=timers.find(k);
    if (it != timers.end())
        timers.erase (it);
}

void timer::stop_all_timers()
{
    timers.clear();
}


