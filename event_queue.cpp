#include "event_queue.h"



queue<event_type> event_queue::events;

void event_queue::add_event(event_type event)
{
	event_queue::events.push(event);
}

event_type event_queue::wait_for_event()
{
	while(events.empty());
	event_type e = events.front();
	event_queue::events.pop();
	return e;
}
