#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>
#include <string.h>

int main() {
    int fd = open("main.c", O_RDONLY);

    char lmao[10];
    read(fd, lmao, 10);

    // Get current offset
    off_t offset = lseek(fd, 0, SEEK_CUR);
    
    // Get the size of the whole file
    lseek(fd, 0, SEEK_SET);
    off_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, offset, SEEK_SET);

    char lmfao[size - offset];
    memset(lmfao, 0, size - offset);
    read(fd, lmfao, size - offset);

    printf("Size of file: %ld\nFile (offset by 10 bytes):\n\n%s", size, lmfao);

    return 0;
}

