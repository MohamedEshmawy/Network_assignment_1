#ifndef DATALINK_LAYER_H
#define DATALINK_LAYER_H

#include "protocol.h"
#include "event_queue.h"
#include "timer.h"
#include <thread>
//#include "physical_layer.h"
#include "network_layer.h"

class Datalink_Layer
{
public:

	static bool between(seq_nr a , seq_nr b, seq_nr c);
	static frame send_data (seq_nr frame_nr, seq_nr frame_expected, simulated_packet buffer[]);
};

#endif
