#include "physical_layer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

void Physical_Layer::physical(bool isServer, int port = 8080)
{
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

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
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        while (!Physical_Layer::sent_frames.empty())
        {
            frame x = Physical_Layer::sent_frames.front().first;
            int d = Physical_Layer::sent_frames.front().second;
            Physical_Layer::sent_frames.pop();
            std::this_thread::sleep_for(std::chrono::milliseconds(d));
            send(newsockfd, &x, sizeof(x), 0);
        }
        vector<frame> rec(MAX_FRAMES_NUM);
        while (recv(sockfd, rec.data(), MAX_FRAMES_NUM * sizeof(frame), 0))
            ;

        for (int i = 0; i < rec.size(); i++)
        {
            Physical_Layer::received_frames.push(rec[i]);
        }
        delete &rec;

        close(newsockfd);
    }
    else
    {
        assert(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0);

        vector<frame> rec(MAX_FRAMES_NUM);
        while (recv(sockfd, rec.data(), MAX_FRAMES_NUM * sizeof(frame), 0))
            ;

        for (int i = 0; i < rec.size(); i++)
        {
            Physical_Layer::received_frames.push(rec[i]);
        }
        delete &rec;

        while (!Physical_Layer::sent_frames.empty())
        {
            frame x = Physical_Layer::sent_frames.front().first;
            int d = Physical_Layer::sent_frames.front().second;
            Physical_Layer::sent_frames.pop();
            std::this_thread::sleep_for(std::chrono::milliseconds(d));
            send(newsockfd, &x, sizeof(x), 0);
        }
    }
    close(sockfd);
}

void Physical_Layer::to_physical_layer(frame *f, int delay = 0)
{
    if (!Physical_Layer::sent_frames.empty())
    {
        Physical_Layer::sent_frames.push(make_pair(*f, delay));
    }
}

frame *Physical_Layer::from_physical_layer()
{
    if (!Physical_Layer::received_frames.empty())
    {
        frame x = Physical_Layer::received_frames.front();
        Physical_Layer::received_frames.pop();
        return &x;
    }
    return NULL;
}