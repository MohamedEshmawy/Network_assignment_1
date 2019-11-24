#include "datalink_layer.h"




bool Datalink_Layer::between(seq_nr a , seq_nr b, seq_nr c)
{
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return(true);
    else
        return(false);
}


frame Datalink_Layer::send_data(seq_nr frame_nr, seq_nr frame_expected, simulated_packet buffer[])
{
    simulated_packet sp = buffer[frame_nr];

    frame s; /* scratch variable */
    s.info = sp.pack; /* insert packet into frame */
    s.seq = frame_nr; /* insert sequence number into frame */
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1); /* piggyback ack */
    if(!sp.successfuly_sent);
        //physical_layer::to_physical_layer(&s); /* transmit the frame */

    return s;



}


