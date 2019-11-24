#include "network_layer.h"
#include "event_queue.h"
#include <windows.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

char test_case0[] = "C0C1C2C3C4C5C6C7C8";
char test_case1[] = "S0S1S2S3S4S5S6S7S8";
int* Network_Layer::chosen_test_case;
bool Network_Layer::Network_layer_enabled ;


void  Network_Layer::enable_network_layer()
{
	Network_Layer::Network_layer_enabled = true;
}

void Network_Layer:: disable_network_layer()
{
	Network_Layer::Network_layer_enabled = false;
}

void  Network_Layer::init()
{
	Network_Layer::enable_network_layer();
}


void Network_Layer::create_network(int test_case, int data_ready_interval)
{
	Network_Layer::chosen_test_case = &test_case;
	while (1)
	{
		if ( ( Network_Layer::Network_layer_enabled ) ==true )
			event_queue::add_event(network_layer_ready);

		Sleep(data_ready_interval);
	}
}

void Network_Layer:: from_network_layer(simulated_packet *p)
{
	static int count = 0;
	char* chosen_tests;
	switch (*chosen_test_case)
	{
		case 0: chosen_tests = test_case0; break;
		case 1: chosen_tests = test_case1; break;
		//case 2: chosen_tests = test_case2; break;
		//case 3: chosen_tests = test_case3; break;
		//case 4: chosen_tests = test_case4; break;
		//case 5: chosen_tests = test_case5; break;
		//case 6: chosen_tests = test_case6; break;
		//case 7: chosen_tests = test_case7; break;
	}
	p->pack.data[0] = chosen_tests[count++];
	p->pack.data[1] = chosen_tests[count++];
	p->pack.data[2] = '\0';
}
