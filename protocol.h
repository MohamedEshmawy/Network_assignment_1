#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_PKT 2            /* determines packet size in bytes */
#define MAX_SEQ 7
#define ACK_TIME_OUT 50
typedef unsigned int seq_nr; /* sequence or ack numbers */
typedef struct
{
    unsigned char data[MAX_PKT+1];
} packet; /* packet definition */
typedef struct
{
    packet pack;
    int delay;
    bool successfuly_sent;
} simulated_packet; /* packet definition */
typedef enum
{
    data,
    ack,
    nak
} frame_kind; /* frame kind definition */
typedef enum
{
    frame_arrival,
    cksum_err,
    timeout,
    network_layer_ready,
    ack_timeout
} event_type;
typedef struct
{
    frame_kind kind; /* what kind of frame is it? */
    seq_nr seq;      /* sequence number */
    seq_nr ack;      /* acknowledgement number */
    packet info;     /* the network layer packet */
} frame;



/* Macro inc is expanded in-line: increment k circularly. */
#define inc(k)       \
    if (k < MAX_SEQ) \
        k = k + 1;   \
    else             \
        k = 0

#endif
