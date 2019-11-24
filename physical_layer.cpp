#include <ws2tcpip.h>
#include "physical_layer.h"
#include <sys/types.h>
#include <Winsock2.h>
//#include <Ws2tcpip.h>

#include <assert.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

int inet_pton(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
    return 1;
      case AF_INET6:
    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
    return 1;
    }
  }
  return 0;
}

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
  struct sockaddr_storage ss;
  unsigned long s = size;

  ZeroMemory(&ss, sizeof(ss));
  ss.ss_family = af;

  switch(af) {
    case AF_INET:
      ((struct sockaddr_in *)&ss)->sin_addr = *(struct in_addr *)src;
      break;
    case AF_INET6:
      ((struct sockaddr_in6 *)&ss)->sin6_addr = *(struct in6_addr *)src;
      break;
    default:
      return NULL;
  }
  /* cannot direclty use &size because of strict aliasing rules */
  return (WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, dst, &s) == 0)?
          dst : NULL;
}




queue<frame> Physical_Layer::received_frames;
//queue<pair<frame, int>> Physical_Layer::sent_frames;
int Physical_Layer::sockfd;
int Physical_Layer::newsockfd;


void Physical_Layer::physical(bool isServer, int port)
{
    WSADATA wsaData;
    WSAStartup(0x202, &wsaData);

    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    frame rec;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* setup the host_addr structure for use in bind call */
    // server byte order
    serv_addr.sin_family = AF_INET;

    // automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // convert short integer value for port must be converted into network byte order
    serv_addr.sin_port = htons(port);

    if (isServer)
    {
        assert(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0);

        listen(sockfd, 50);


        clilen = sizeof(cli_addr);

        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        sockfd = newsockfd;
        event_queue::add_event(connection_established);
    }
    else
    {

        //assert(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0);
        inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
        while(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));
            //cout << connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) << endl;

        event_queue::add_event(connection_established);
    }

    while(1)
    {
        if(recv(sockfd, (char*)&rec, sizeof(frame), 0))
        {
            received_frames.push(rec);
            event_queue::add_event(frame_arrival);
        }
    }

}

void Physical_Layer::to_physical_layer(frame *f, int delay)
{

    frame x = *f;
    int d = delay;
    //std::this_thread::sleep_for(std::chrono::milliseconds(d));
    send(sockfd, (char*)(&x), sizeof(x), 0);
}

frame Physical_Layer::from_physical_layer()
{
    //if (!Physical_Layer::received_frames.empty())
    {
        frame x = Physical_Layer::received_frames.front();
        Physical_Layer::received_frames.pop();
        return x;
    }

}
