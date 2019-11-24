#include "network_layer.h"
#include "event_queue.h"
#include <iostream>
#include <windows.h>
using namespace std;

//char test_case0[] = "CACBCDCECFCGCHCICK";
//char test_case1[] = "SASBSCSDSESFSGSHSI";
char test_case0[] = "HELLO_MY_FRIEND_GO";
char test_case1[] = "HOW_ARE_YOU_FRIEND";
char test_case4[] = "W0W1W2W3W4W5W6W7W8";
char test_case5[] = "Z0Z1Z2Z3Z4Z5Z6Z7Z8";
char test_case6[] = "Q0Q1Q2Q3Q4Q5Q6Q7Q8";
char test_case7[] = "X0X1X2X3X4X5X6X7X8";
bool successfuly_sent[] = { true, true, true, false, true, true, true,true,true };
int Network_Layer::chosen_test_case;
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
	Network_Layer::chosen_test_case = test_case;
	while (1)
	{
		if ( ( Network_Layer::Network_layer_enabled ) ==true )
        {
			event_queue::add_event(network_layer_ready);
        }

		Sleep(data_ready_interval/2 + rand() % (data_ready_interval/2));
	}
}

void Network_Layer:: from_network_layer(simulated_packet *p)
{
	static int count = 0;
	char* chosen_tests;
	switch (chosen_test_case)
	{
		case 0: chosen_tests = test_case0; break;
		case 1: chosen_tests = test_case1; break;
		case 4: chosen_tests = test_case4; break;
		case 5: chosen_tests = test_case5; break;
		case 6: chosen_tests = test_case6; break;
		case 7: chosen_tests = test_case7; break;
	}
	p->pack.data[0] = chosen_tests[count];
	count = (count + 1) % 18;
	p->pack.data[1] = chosen_tests[count];
	count = (count + 1) % 18;
	p->pack.data[2] = '\0';
	//p->successfuly_sent = successfuly_sent[int(count / 2)];
    p->successfuly_sent =    rand() % 100 < 70;
}
