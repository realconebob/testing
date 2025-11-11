// Notify the user if a file has been modified

// This stuff is NOT portable and I don't care it if is
#define _GNU_SOURCE

// Inotify api & funcs, dealing with filedescs
#include <sys/inotify.h>
#include <sys/types.h>
#include <unistd.h>

// Error handling
#include <error.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Macro Definitions
#define TESTFILE "test"

#define E_INOTIFY_CANTINIT 1
#define E_INOTIFY_CANTWATCH 2
#define E_INOTIFY_CANTRM 3
#define E_INOTIFY_CANTCLOSE 4
#define E_INOTIFY_CANTREAD 5

#define P_IN_EVENT_SIZE (sizeof(struct inotify_event) + FILENAME_MAX + 1)
#define P_IN_EVENTS 1

struct in_container {
    int filedesc;   // File descriptor created when using inotify_init()
    int watchdesc;  // Watch descriptor created when using inotify_add_watch
    char *ebuffer;   // Buffer for storing events

};

// Borrowed from https://www.gnu.org/software/libc/manual/html_node/Malloc-Examples.html
// Allocate memory with calloc, exiting on failure
void* xcalloc(size_t nmemb, size_t size) {
    void *mem = calloc(nmemb, size);
    if(mem == NULL)
        error(-1, errno, "Virtual memory exhausted");

    return mem;
}

// Create a new inotify instance and assign a file to it
int init_incontainer(struct in_container *CONT, const char* PATH, uint32_t mask, size_t events) {
    // Allocate some memory for the ebuffer
    CONT->ebuffer = xcalloc(events, P_IN_EVENT_SIZE);

    // Initalize inotify instance
    CONT->filedesc = inotify_init();
    if(CONT->filedesc < 0) {
        error(0, errno, "Could not initalize inotify instance");
        return E_INOTIFY_CANTINIT;
    }

    // Add file PATH into the watch list
    CONT->watchdesc = inotify_add_watch(CONT->filedesc, PATH, mask);
    if(CONT->watchdesc < 0) {
        error(0, errno, "Could not watch file \"%s\"", PATH);
        return E_INOTIFY_CANTWATCH;
    }

    return 0;
}

int rm_incontainer(struct in_container *CONT) {
    // Clear the watch descriptor
    if(inotify_rm_watch(CONT->filedesc, CONT->watchdesc) < 0) {
        error(0, errno, "Couldn't clear watch descriptor for in_container instance");
        return E_INOTIFY_CANTRM;
    }

    // Close the file descriptor created by inotify_init
    if(close(CONT->filedesc) < 0) {
        error(0, errno, "Couldn't close in_container file desc");
        return E_INOTIFY_CANTCLOSE;
    }

    // Free the event buffer
    free(CONT->ebuffer);
    return 0;
}

int main() {
    int status; // Me when the return value checking

    // Create an in_container instance & initalize it
    struct in_container container;
    if((status = init_incontainer(&container, TESTFILE, IN_MODIFY, P_IN_EVENTS)) != 0)
        exit(status);

    // Read the file descriptor for an update
    if(read(container.filedesc, container.ebuffer, P_IN_EVENT_SIZE * P_IN_EVENTS) < 0)
        error(E_INOTIFY_CANTREAD, errno, "Couldn't read inotify file desc");

    struct inotify_event *event = (struct inotify_event*)&container.ebuffer;
    if(event->mask && IN_MODIFY)
        printf("File modified!\n");

    // Clean up in_container instance now that we're done using it
    if((status = rm_incontainer(&container)) != 0)
        exit(status);

    return 0;
}