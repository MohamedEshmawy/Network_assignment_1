#ifndef TIMER_H
#define TIMER_H

#include "protocol.h"
#include <set>

using namespace std;

class timer
{
public:
	/* starts timer associated with the specified seq number */
	static void start_timer(seq_nr s, int time);

	/* stops timer associated with the specified seq number */
	static void stop_timer(seq_nr k);

	static void stop_all_timers();

private:
	/*
		set containing timers seq numbers
	*/
	static set<seq_nr> timers;
};

#endif
