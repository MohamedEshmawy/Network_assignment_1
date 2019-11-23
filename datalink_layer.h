#ifndef DATALINK_LAYER_H
#define DATALINK_LAYER_H

#include "protocol.h"
#include "event_queue.h"
#include "physical_layer.h"
#include "network_layer.h"

class Network_Layer
{
public:
	/* Fetch a packet from the network layer for transmission on the channel. */
	static void from_network_layer(simulated_packet *p);

	/* Deliver information from an inbound frame to the network layer. */
	static void to_network_layer(packet *p);

	/*
			specify which test case to use and the rate at which data is ready
			and contains a while loop in which it generates network ready events
		*/
	static void create_network(int test_case, int data_ready_interval);

	/* Allow the network layer to cause a network layer ready event. */
	static void enable_network_layer();

	/* Forbid the network layer from causing a network layer ready event. */
	static void disable_network_layer();

private:
	/*
			if false the network layer will stop to push events into the event queue
		*/
	static bool Network_layer_enabled;
	/*
			the index of the test case to be used
		*/
	static int chosen_test_case;
};

#endif