#include <windows.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <set>
#include <queue>
#include <thread>
#include "protocol.h"
#include "event_queue.h"
#include "datalink_layer.h"
#include "timer.h"
#include "network_layer.h"
#include "physical_layer.h"
#include <windows.h>

using namespace std;

//terminal colors
HANDLE hConsol = GetStdHandle(STD_OUTPUT_HANDLE);;
#define RED     SetConsoleTextAttribute(hConsol, 4);      /* Red */
#define Green     SetConsoleTextAttribute(hConsol, 2);      /* Green */
#define CYAN     SetConsoleTextAttribute(hConsol, 3);      /* Cyan */
#define WHITE     SetConsoleTextAttribute(hConsol, 15);      /* White */


void print_sent_frame(frame sent, bool success, int nbuffered)
{
printf("sending.. data: %s , seq_nr: %d , ack: %d , window buffer: %d\n",
                       sent.info.data , sent.seq , sent.ack , nbuffered );
}
void print_received_frame(frame rec, bool discarded)
{
    Green
    if (discarded)
                printf("received.. data: %s , seq_nr: %d , ack: %d \t [DISCARDED]\n",
                       rec.info.data , rec.seq , rec.ack);
    else
        printf("received.. data: %s , seq_nr: %d , ack: %d\n",
                       rec.info.data , rec.seq , rec.ack);
    WHITE
}


int main()
{
    //user input
    int chosen_testcase;
    char answer;
    bool isServer;
    cout<<"Are you a server? (y/n):";
    cin >> answer;
    cout << endl;
    if (answer == 'y')
        isServer = true;
    else
        isServer = false;

    thread thread_object2(Physical_Layer::physical, isServer, 8090);
    event_type event = event_queue::wait_for_event();
    if (event == connection_established) cout<< "Connected\n";

    Network_Layer::enable_network_layer();
    thread thread_object(Network_Layer::create_network, isServer,SEND_INTERVAL);

    seq_nr next_frame_to_send; /* MAX SEQ > 1; used for outbound stream */

    seq_nr ack_expected; /* oldest frame as yet unacknowledged */
    seq_nr frame_expected; /* next frame expected on inbound stream */

    frame r; /* scratch variable */
    simulated_packet buffer[MAX_SEQ + 1]; /* buffers for the outbound stream */
    seq_nr nbuffered; /* number of output buffers currently in use */
    seq_nr i; /* used to index into the buffer array */


    ack_expected = 0; /* next ack expected inbound */
    next_frame_to_send = 0; /* next frame going out */
    frame_expected = 0; /* number of frame expected inbound */
    nbuffered = 0; /* initially no packets are buffered */

    while (true) {
        event = event_queue::wait_for_event();
        Network_Layer:: disable_network_layer();

        switch(event) {
            case network_layer_ready: /* the network layer has a packet to send */
            {
                /* Accept, save, and transmit a new frame. */

                Network_Layer::from_network_layer(&buffer[next_frame_to_send]); /* fetch new packet */
                nbuffered = nbuffered + 1; /* expand the sender’s window */

                frame sent = Datalink_Layer::send_data(next_frame_to_send, frame_expected, buffer);/* transmit the frame */
                print_sent_frame(sent, 0, nbuffered);
                inc(next_frame_to_send); /* advance sender’s upper window edge */




                break;
            }
            case frame_arrival: /* a data or control frame has arrived */
            {
                r = Physical_Layer::from_physical_layer(); /* get incoming frame from physical layer */

                if (r.seq == frame_expected) {
                /* Frames are accepted only in order. */
                //Network_Layer::to_network_layer(&r.info); /* pass packet to network layer */
                print_received_frame(r,0);
                inc(frame_expected); /* advance lower edge of receiver’s window */
                }
                else
                    print_received_frame(r,1);
                /* Ack n implies n − 1, n − 2, etc. Check for this. */
                while (Datalink_Layer::between(ack_expected, r.ack, next_frame_to_send)) {
                /* Handle piggybacked ack. */
                nbuffered = nbuffered - 1; /* one frame fewer buffered */
                timer::stop_timer(ack_expected); /* frame arrived intact; stop timer */
                inc(ack_expected); /* contract sender’s window */
                }
                break;
            }
            case cksum_err: break; /* just ignore bad frames */
            case ack_timeout: /* trouble; retransmit all outstanding frames */
                timer::stop_all_timers();
                RED printf("Acknowledgment time out for frame %d, resending buffered frames....\n" , ack_expected); WHITE
                next_frame_to_send = ack_expected; /* start retransmitting here */
                for (i = 1; i <= nbuffered; i++) {
                    frame sent = Datalink_Layer::send_data(next_frame_to_send, frame_expected, buffer);/* resend frame */
                    print_sent_frame(sent, true, nbuffered);
                    Sleep(10);
                    inc(next_frame_to_send); /* prepare to send the next one */
                    }
                CYAN printf("Resuming normal sending\n"); WHITE

    }
    if (nbuffered < MAX_SEQ)
        Network_Layer::enable_network_layer();
    else
        Network_Layer::disable_network_layer();
    }


    /*
    //event queue test
    event_queue::add_event(frame_arrival);
    event_type e = event_queue::wait_for_event();
    cout << e;
    */

    //threading and timer test
    /*
    thread thread_object(timer::start_timer, 1,5000);
    cout<<"hi";
    //timer::stop_timer(1);
    event_type e = event_queue::wait_for_event();
    cout << e;
    */

    //physical layer test
    /*
    char answer;
    bool isServer;
    cout<<"Are you a server? (y/n):";
    cin >> answer;
    cout << endl;
    if (answer == 'y')
        isServer = true;
    else
        isServer = false;

    frame send_f, receive_f;
    send_f.seq = 1;
    send_f.info.data[0] = 'x';
    send_f.info.data[1] = 'z';
    send_f.info.data[2] = '\0';
    send_f.ack = 6;

    thread thread_object(Physical_Layer::physical, isServer, 8080);
    event_type event = event_queue::wait_for_event();
    if (event == connection_established) cout<< "Connected\n";

    Physical_Layer::to_physical_layer(&send_f, 0);
    cout<< "sent\n";

    event = event_queue::wait_for_event();
    if (event == frame_arrival) cout<< "arrived\n";
    receive_f = Physical_Layer::from_physical_layer();
    print_frame(receive_f, 0, 0);
    */

    return 0;
}
