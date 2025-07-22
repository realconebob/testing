#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/util.h>
#include <event2/dns.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <error.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Design strategy:
    //  1- Create an listener using libevent [DONE]
    //  2- Once a connection is opened, create an bufferevent for it, and put it in the base loop [DPNE]
    //  3- Handle connections until one of them EOFs or runs into an error [DONE]
        //  3.1- If there's an error or EOF, close the connection by deleting the bufferevent [DONE]

    //  4- Wait for input from the user on STDIN
    //  5- Once there's input, push it to all connected clients

*/

/* bufferevent read and write callbacks
// So, the bufferevent read and write callbacks are simultaneously simple and annoying:
    // The read callback will execute the moment there's new data to be read on the socket
    // The write callback will execute the moment the output buffer has sent all it's data

// Now, with the write callback, you'd maybe think that if the buffer has nothing in it, as there's been
// nothing put in it yet, AND the socket is ready for writing, that it would trigger a write callback, right?
// WRONG! THAT WOULD MAKE TOO MUCH SENSE! SUCH BEHAVIOR ONLY WORKS FOR THE SERVER SIDE OF THE CONNECTION,
// PROBABLY BECAUSE OF HOW THE LISTENER EVENT WORKS! (REGISTERS A WRITE EVENT ONCE IT BECOMES CONNECTED/WRITABLE)

// No, instead the write buffer must have something put in it BEFORE any more write callbacks fire. This means
// you have to "prime" the buffer by sending something on connection or during a read callback. Did the
// documentation explain this? Yes, but not well. Did it take me far too long to figure this out? Probably.

// */

#define BACKLOG -1
#define STATIC_ARRSIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

int stdinforeach(const struct event_base *base, const struct event *event, void *ctx) {
    
    
    return 0;
}

void stdincallback(evutil_socket_t fd, short what, void *ctx) {
    event_base_foreach_event((struct event_base*)ctx, stdinforeach, NULL);

    return;
}


void client_readcallback(struct bufferevent *bev, void *ctx) {
    printf("Read callback fired\n");

    struct evbuffer *input = bufferevent_get_input(bev);
    if(!input)
        error(1, 0, "Couldn't get evbuffer");

    char stuff[512]; size_t evbuflen = -1;
    
    size_t sizetoread = (evbuffer_get_length(input) <= STATIC_ARRSIZE(stuff)) ? evbuffer_get_length(input) : STATIC_ARRSIZE(stuff);

    bufferevent_read(bev, stuff, sizetoread);
    printf("\tGot: %s\n", stuff);

    return;
};

void client_writecallback(struct bufferevent *bev, void *ctx) {
    printf("Write callback fired\n");

    return;
};

void client_eventcallback(struct bufferevent *bev, short events, void *ctx) {
    printf("Event callback fired\n");

    struct event_base *base = bufferevent_get_base(bev);
    printf("\t> Dumping current events:\n> ========== <\n");
    event_base_dump_events(base, stdout);
    printf("> ========== <\n");


    printf("\tGot event: %s%s%s%s%s%s\n",
        (events & BEV_EVENT_READING)    ? "(Something happened during a read) "    : "",
        (events & BEV_EVENT_WRITING)    ? "(Something happened during a write) "   : "",

        (events & BEV_EVENT_CONNECTED)  ? "Connected to remote socket"          : "",
        (events & BEV_EVENT_EOF)        ? "End of File received"                : "",
        (events & BEV_EVENT_ERROR)      ? "Ran into an error"                   : "" ,
        (events & BEV_EVENT_TIMEOUT)    ? "A timeout has expired"               : "" 
    );
    if(events & BEV_EVENT_ERROR)
        error(1, EVUTIL_SOCKET_ERROR(), "Fatal error");

    if(events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        printf("\tRemoved client from base (%s%s%s)\n",
            (events & BEV_EVENT_EOF)        ? "End of File"     : "",
            (events & BEV_EVENT_ERROR)      ? "Socket Error"    : "",
            (events & BEV_EVENT_TIMEOUT)    ? "Timeout"         : ""
        );
        bufferevent_free(bev); // If you free the bufferevent, it stops executing things IMMEDIATELY
    }

    return;
};



void connectioncallback(struct evconnlistener *listener, evutil_socket_t sock, struct sockaddr *addr, int len, void *ptr) {
    printf("New client detected\n");

    struct event_base *base = evconnlistener_get_base(listener);
    if(!base) {
        error(0, 0, "Couldn't get base for adding a new connection");
        return;
    }

    struct bufferevent *newclient = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE /*| BEV_OPT_DEFER_CALLBACKS*/ /*| BEV_OPT_THREADSAFE*/);
    if(!newclient) {
        error(0, 0, "Couldn't create a new bufferevent for an incomming connection");
        return;
    }

    bufferevent_setcb(newclient, client_readcallback, client_writecallback, client_eventcallback, NULL);
    bufferevent_enable(newclient, EV_READ | EV_WRITE);
    
    return;
}

void evcl_errorcb(struct evconnlistener *lis, void *ptr) {
    struct event_base *base = evconnlistener_get_base(lis);
    error(0, EVUTIL_SOCKET_ERROR(), "Got an error on the listener socket");

    event_base_loopexit(base, NULL);
    
    return;
}

int main() {
    struct evconnlistener *listener = NULL;
    struct event_base *based = NULL;

    // Create event base
    struct event_config *cfg = event_config_new();
    if(!cfg)
        error(1, 0, "Couldn't get event_config object");
    event_config_require_features(cfg, EV_FEATURE_FDS);
    event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP | EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
 
    based = event_base_new_with_config(cfg);
    if(!based)
        error(1, 0, "Could not create event base");
    event_config_free(cfg);


    // Use gai to grab necessary things
    struct evutil_addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0,
        .ai_flags = EVUTIL_AI_PASSIVE
    }, *res;
    int err = evutil_getaddrinfo("localhost", "26116", &hints, &res);
    if(err != 0)
        error(1, 0, "Couldn't get address info: %s", evutil_gai_strerror(err));

    

    // Loop through the linked list gai provides
    struct evutil_addrinfo *p = NULL;
    for(p = res; p != NULL; p = p->ai_next) {

        // Create & bind to listener
        listener = evconnlistener_new_bind(based, connectioncallback, NULL, 
                                            LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE | LEV_OPT_THREADSAFE, 
                                            BACKLOG, p->ai_addr, p->ai_addrlen);

        if(!listener) {
            error(0, 0, "Couldn't bind to socket");
            continue;
        }

        break;
    }
    if(p == NULL)
        error(1, 0, "Couldn't bind to anything");
    evutil_freeaddrinfo(res);


    // Set the error handling callback
    evconnlistener_set_error_cb(listener, evcl_errorcb);

    // Enable the listener & enter main loop
    evconnlistener_enable(listener);





    /* Note: I'll need these eventually

    typedef int (*event_base_foreach_event_cb)(const struct event_base *, const struct event *, void *);
    int event_base_foreach_event(struct event_base *base, event_base_foreach_event_cb fn, void *arg);

    //*/

    // struct event *stdinreader = event_new(based, STDIN_FILENO, EV_READ | EV_PERSIST, stdincallback, (void*)based);


        /*// NOTE:
        // I've decided that I don't care about finishing this right now. The implementation I'd use would include a
        // a linked list and I don't want to have to implement one for this as of this moment. Maybe I'll come back
        // to this project after a month or something and just hack it out in 2 days; Maybe not
        //*/


    // Start the loop
    event_base_dispatch(based);
    event_base_free(based);

    return 0;
}