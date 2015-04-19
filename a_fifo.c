/* 14:17 2015-04-19 Sunday */
#include <head.h>

void read_fifo(int r_fd)
{
    int n;
    char buf[1024];

    while (1)
    {

        n =  read(r_fd,buf,sizeof(buf));
        buf[n] = '\0';
        fprintf(stdout,"Read %d types from %d : %s\n",n,r_fd,buf);

        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }
    }
    return;
}

int main(int argc, const char *argv[])
{
    int r_fd;

    if (argc < 2)
    {
        fprintf(stderr,"Usage : %s read_fifo\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    if (mkfifo(argv[1],0666) != 0 && errno != EEXIST)
    {
        perror("Fail to mkfifo");
        exit(EXIT_FAILURE);
    }
    
    r_fd = open(argv[1],O_RDONLY);
    if (r_fd < 0)
    {
        perror("Fail to open");
        exit(EXIT_FAILURE);
    }
    read_fifo(r_fd);

    return 0;
}
