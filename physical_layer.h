#ifndef PHYSICAL_LAYER_H
#define PHYSICAL_LAYER_H

#include "protocol.h"

#include "event_queue.h"

using namespace std;

#include <queue>

class Physical_Layer
{
public:
	/*
			sends data to the other side
		*/
	static void to_physical_layer(frame *f, int delay=0);

	/*
			returns the head of the received frames queue and pops the queue
		*/
	static frame from_physical_layer();

	/*
			either creates or connects to a local tcp socket given its port
			and contains a while loop in which it listens to data from the other side
			it pushes the data received in the recevied frames queue
		*/
	static void physical(bool isServer, int port=8080);

private:
	/*
		queue containing received frames
	*/
	static queue<frame> received_frames;
	//static queue<pair<frame, int>> sent_frames;
	static int sockfd, newsockfd;

};

#endif
