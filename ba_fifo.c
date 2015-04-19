/* 14:30 2015-04-19 Sunday */
#include <head.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void read_fifo(int r_fd)
{
    int n;
    char buf[1024];

    while (1)
    {

        pthread_cond_wait(&cond,&lock);
        n =  read(r_fd,buf,sizeof(buf));
        buf[n] = '\0';
        fprintf(stdout,"Read %d types from %d : %s\n",n,r_fd,buf);

        pthread_mutex_unlock(&lock);
        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }
    }
    return;
}
void write_fifo(int w_fd)
{
    int n;
    char buf[1024];

    while (1)
    {
        pthread_mutex_lock(&lock);
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';

        write(w_fd,buf,strlen(buf));

        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&lock);
        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }
        usleep(500);

    }
    return;
}

int main(int argc, const char *argv[])
{
    int w_fd; 
    int r_fd;
    pid_t pid;

    if (argc < 3)
    {
        fprintf(stderr,"Usage : %s write_fifo read_fifo\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    if (mkfifo(argv[1],0666) != 0 && errno != EEXIST)
    {
        perror("Fail to mkfifo");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(argv[2],0666) != 0 && errno != EEXIST)
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
    r_fd = open(argv[2],O_RDONLY);
    if (r_fd < 0)
    {
        perror("Fail to open");
        exit(EXIT_FAILURE);
    }
    printf("w_fd = %d\n",w_fd);

    pid = fork();
    if (pid < 0)
    {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        read_fifo(r_fd);
    }
    if (pid > 0)
    {
        write_fifo(w_fd);
    }

    return 0;
}
