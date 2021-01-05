#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    puts("Entering client");
    int fd = open("/dev/encoder", O_RDWR);
    if (fd < 0)
    {
        printf("%m\n");
        return -1;
    }
    union {
        char bytes[sizeof(long)];
        long value;
    } data;
    int rd_bytes = read(fd, data.bytes, sizeof(long));
    if (rd_bytes != sizeof(long)) { printf("Read %d bytes !\n", rd_bytes); }
    else
    {
        printf("Read: %ld\n", data.value);
    }
    close(fd);

    return 0;
}
