#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#define SA struct sockaddr
#define SIZE 500

void getfile(char *, char *);
void listFiles(const char *path);
// void makefile(char *,char *);
void copy(char b[], int beg, char t[])
{
    int i;
    for (i = beg; b[i] != '\n'; i++)
        t[i - beg] = b[i];
}
void error(char *msg)
{
    perror(msg);
    exit(1);
}

void server(int connfd)
{
    int no_of_bytes;
    char buffer[SIZE], temp[SIZE];
    bzero(buffer, SIZE);
    while (1)
    {
        no_of_bytes = read(connfd, buffer, SIZE - 1);

        if (no_of_bytes < 0)
            error("Read");
        else if (no_of_bytes == 0)
        {

            printf("\nClient Disconnected\n");
            // bzero(buffer, SIZE);
        }


        printf("\nMessage from Client: %s - Number of bytes: %d ", buffer, no_of_bytes);

        // listFiles("/dummy");
        bzero(temp, SIZE);
        strcpy(temp,buffer);
        bzero(buffer, SIZE);
        getfile(buffer, temp);
        no_of_bytes = write(connfd, buffer, strlen(buffer));

        if (no_of_bytes < 0)
            error("Write");

        bzero(buffer, SIZE);
        // close(connfd);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("No Port Provided\n");
        exit(1);
    }

    int portno = atoi(argv[1]);
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portno);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    while (1)
    {

        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (connfd < 0)
        {
            printf("server accept failed...\n");
            exit(0);
        }
        else
            printf("server accept the client...\n");
        if (fork() == 0)
        {

            server(connfd);
        }
    }
}

void makefile(char *array, char *temp)
{
    FILE *fp;
    char ch;
    int i = 0;
    fp = fopen(temp, "w");
    while (1)
    {
        ch = array[i++];
        if (ch == '\0')
            break;
        fputc(ch, fp);
    }
    fclose(fp);
}

void listFiles(const char *path)
{
    struct dirent *dp;
    DIR *dir = opendir(path);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        printf("%s\n", dp->d_name);
    }

    // Close directory stream
    closedir(dir);
}

void getfile(char *array, char *temp)
{
    // printf("array: %s, temp: %s \n", array, temp);

    FILE *fp;
    char ch;
    int i = 0;
    fp = fopen(temp, "r");
    if (fp == NULL)
    {
        strcpy(array, "No such file in Server Directory\n");
        perror("");
    }
    else
    {
        while (1)
        {
            ch = fgetc(fp);
            if (ch == EOF)
                break;
            array[i++] = ch;
        }
        fclose(fp);
    }
}