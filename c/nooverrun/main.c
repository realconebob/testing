int main() {

}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>

#include <errno.h>
#include <error.h>


int persistentread(int fin, void *buf, size_t *nbytes) {
    ssize_t bytesread = 0;
    size_t curlen = 0;

    int errsave = 0;
    while((bytesread = read(fin, ((uint8_t*)buf) + curlen, *nbytes - curlen)) > 0) {
        errsave = errno;
        curlen += bytesread;
        if(curlen > *nbytes)
            error(1, ERANGE, "Read too many bytes somehow");
    }
    if(bytesread < 0)
        error(1, errsave, "read() error");

    *nbytes = curlen;

    return 0;
}

int dontOverrunAFile() {
    const char *finname = "main.c";

    int fdin = -1;
    struct stat sb;
    if((fdin = open(finname, O_RDONLY)) < 0)
        error(1, errno, "open() error");
    if(fstat(fdin, &sb))
        error(1, errno, "stat() error");

    const size_t size = 30;
    size_t nbytes = size;
    char *fcontents = malloc(nbytes);
    if(!fcontents)
        error(1, errno, "malloc() error");

    // Do reading here
    // while(nbytes >= size) {
    //     persistentread(fdin, fcontents, &nbytes);

    //     if(nbytes < size)
    //         fcontents[nbytes] = '\0'; // Bonus: This will never be an oob error, assuming your block size and buffer size are equal (or buffer > block)
    //     printf("%s", fcontents);
    // }

    off_t total = 0;
    while(total < sb.st_size) {
        nbytes = size;
        persistentread(fdin, fcontents, &nbytes);
        total += nbytes;

        if(nbytes < size && total >= sb.st_size)
            fcontents[nbytes] = '\0';

        printf("%s", fcontents);
        
    }

    close(fdin);

    return 0;

    /* Once the number of bytes you're reading is less than the block size you're using, there's less than that
    // many bytes left in the file, so you're done reading it. Then the only challenge is printing out exactly
    // the code you have, as anything left in the buffer after your block size will still be there */
}