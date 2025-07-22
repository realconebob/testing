#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/util.h>
#include <event2/dns.h>

#include <sodium.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <error.h>

#include <stdio.h>
#include <stdlib.h>

#define STATIC_ARRSIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))


void write_callback(struct bufferevent *bev, void *ctx) {
    printf("Write callback fired\n");

    return;
}
void read_callback(struct bufferevent *bev, void *ctx) {
    printf("Read callback fired\n");

    return;
}
void event_callback(struct bufferevent *bev, short events, void *ctx) {
    printf("Event callback fired\n");

    printf("Got event: %s%s%s%s%s%s\n",
        (events & BEV_EVENT_READING)    ? "(Something happened during a read) "    : "",
        (events & BEV_EVENT_WRITING)    ? "(Something happened during a write) "   : "",

        (events & BEV_EVENT_CONNECTED)  ? "Connected to remote socket"          : "",
        (events & BEV_EVENT_EOF)        ? "End of File received"                : "",
        (events & BEV_EVENT_ERROR)      ? "Ran into an error"                   : "" ,
        (events & BEV_EVENT_TIMEOUT)    ? "A timeout has expired"               : "" 
    );

    if(events & BEV_EVENT_ERROR)
        error(1, EVUTIL_SOCKET_ERROR(), "Fatal error");

    if(events & BEV_EVENT_CONNECTED)
        bufferevent_write(bev, "lmao", 5 * sizeof(char));
}


int main() {
    struct event_base *based = NULL;
    struct bufferevent *servercon = NULL;

    // Get the event base
    based = event_base_new();
    if(!based)
        error(1, 0, "Couldn't get event base");

    // Create a new bufferevent object
    servercon = bufferevent_socket_new(based, -1, BEV_OPT_CLOSE_ON_FREE /*| BEV_OPT_DEFER_CALLBACKS*/ /* No threadsafe because it crashes */);
    if(!servercon)
        error(1, 0, "Couldn't create bufferevent object");


    // Connect to the remote server
        /* dnsbase is null because despite what libevent thinks, I do not have a seething hatred of all things that
        // block. I only care about non-blocking reading/writing to sockets, not removing all blocking behavior entirely*/ 
    if(bufferevent_socket_connect_hostname(servercon, NULL, AF_UNSPEC, "localhost", 26116) < 0)
        error(1, 0, "Couldn't connect to remote host");

    bufferevent_setcb(servercon, read_callback, write_callback, event_callback, NULL);
    bufferevent_enable(servercon, EV_READ | EV_WRITE);

    event_base_dispatch(based);

    return 0;
}