#include <stdio.h>
#include <string.h>

int deltaencode(char *buffer, int length) {
    int last = 0;
    for(int i = 0; i < length; i++) {
        int current = buffer[i];
        buffer[i] = current - last;
        last = current;
    }

    return 0;
}

int deltadecode(char *buffer, int length) {
    int last = 0;
    for(int i = 0; i < length; i++) {
        int delta = buffer[i];
        buffer[i] = delta + last;
        last = buffer[i];
    }
    return 0;
}

int main() {
    char *teststring = strdup("testing");
    int length = strlen(teststring);
    
    deltaencode(teststring, length);
    printf("%s\n", teststring);

    deltadecode(teststring, length);
    printf("%s\n", teststring);
    return 0;
}