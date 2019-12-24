#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];
int main()
{
    int fd, ret;
    char msg[BUFFER_LENGTH];
    printf("Test program starts...\n");
    fd = open("/dev/buffer", O_RDWR);
    if(fd < 0) {
        perror("Device file opening failed\n");
        return errno;
    }

    printf("Enter a short message to write it into device file...\n");
    scanf("%[^\n]%*c", msg);
    //fgets(msg, 20, stdin);
    printf("Message is being pushed to buffer...\n");
    ret = write(fd, msg, strlen(msg));
    printf("Driver takes in %d bytes\n\n", ret);
    if(ret < 0) {
        perror("Failed writing\n");
        return errno;
    }

    printf("Press ENTER to launch reading from device file...\n");
    getchar();
    printf("Reading starts...\n");
    ret = read(fd, receive, BUFFER_LENGTH);
    printf("Driver feeds back %d bytes\n", ret);
    if(ret < 0) {
        perror("Failed reading\n");
        return errno;
    }
    printf("Message read from device file: %s\n", receive);

    return 0;
}


