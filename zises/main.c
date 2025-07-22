// Read a file that increases in size, then find the new size of that file & read it
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    off_t newbytes, oldbytes, offset;


    int file = open("test", O_RDONLY);
    oldbytes = offset = lseek(file, 0, SEEK_END);

    int infd = inotify_init();
    int wd = inotify_add_watch(infd, "test", IN_MODIFY);
    
    struct inotify_event ebuff;
    int r = read(infd, &ebuff, sizeof(ebuff));

    if(r > 0) {
        //sleep(1);
        newbytes = lseek(file, 0, SEEK_END);
        lseek(file, offset, SEEK_SET);

        printf("Old size: %ld\nNew size: %ld\nDifference: %ld", oldbytes, newbytes, newbytes - oldbytes);

    }
    
    inotify_rm_watch(infd, wd);
    close(infd);
    close(file);

    return 0;
}