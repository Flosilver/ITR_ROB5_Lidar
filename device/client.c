#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    puts("Entering client");
    int fd = open("/dev/encoder", O_RDWR);
    if (fd < 0)
    {
        printf("%m\n");
        return -1;
    }
    union {
        char bytes[4];
        long value;
    } data;
    int rd_bytes = read(fd, data.bytes, 4);
    if (rd_bytes != 4) { printf("Read %d bytes !\n", rd_bytes); }
    else
    {
        printf("Read: %ld\n", data.value);
    }
    close(fd);

    return 0;
}
