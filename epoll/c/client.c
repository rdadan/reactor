#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

void do_service(int sockfd);

int main(int argc, const char *argv[])
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if (peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // addr.sin_addr.s_addr = inet_addr("192.168.40.91");
    addr.sin_port = htons(8888);
    socklen_t len = sizeof addr;
    if (connect(peerfd, (struct sockaddr *)&addr, len) == -1)
        ERR_EXIT("Connect");
    printf("client ip:%s,port:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    do_service(peerfd);

    return 0;
}

void do_service(int sockfd)
{
    char recvbuf[1024] = {0};
    char sendbuf[1024] = {0};
    read(sockfd, recvbuf, sizeof recvbuf);
    printf("receive msg : %s\n", recvbuf);
    while (1)
    {
        memset(recvbuf, 0, sizeof recvbuf);
        memset(sendbuf, 0, sizeof sendbuf);
        fgets(sendbuf, sizeof sendbuf, stdin);
        write(sockfd, sendbuf, strlen(sendbuf));

        // read
        int nread = read(sockfd, recvbuf, sizeof recvbuf);
        if (nread == -1)
        {
            if (errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if (nread == 0)
        {
            printf("server close!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }

        printf("receive msg : %s", recvbuf);
    }
}
