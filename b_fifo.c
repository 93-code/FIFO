/* 14:30 2015-04-19 Sunday */
#include <head.h>

void write_fifo(int w_fd)
{
    int n;
    char buf[1024];

    while (1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';

        write(w_fd,buf,strlen(buf));

        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }

    }
    return;
}

int main(int argc, const char *argv[])
{
    int w_fd; 

    if (argc < 2)
    {
        fprintf(stderr,"Usage : %s write_fifo\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    if (mkfifo(argv[1],0666) != 0 && errno != EEXIST)
    {
        perror("Fail to mkfifo");
        exit(EXIT_FAILURE);
    }

    w_fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0666);
    if (w_fd < 0)
    {
        perror("Fail to open");
        exit(EXIT_FAILURE);
    }
    printf("w_fd = %d\n",w_fd);

    write_fifo(w_fd);

    return 0;
}
