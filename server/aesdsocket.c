#include <arpa/inet.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

int sockfd = -1;
int connfd = -1;
FILE* data_file = NULL;

void handler(int signum) {
    syslog(LOG_INFO, "Caught signal, exiting\n");
    closelog();

    if (connfd > 0) {
        close(connfd);
    }
    if (sockfd > 0) {
        close(sockfd);
    }
    if (data_file) {
        fclose(data_file);
    }

    if (unlink("/var/tmp/aesdsocketdata") == -1) {
        perror("unlink");
    }

    exit(0);
}

int main(void) {
    openlog(NULL, 0, LOG_USER);

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction: SIGINT");
        return -1;
    }
    if (sigaction(SIGTERM, &action, NULL) == -1) {
        perror("sigaction: SIGTERM");
        return -1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("sockfd");
        return -1;
    }

    struct sockaddr_in server_addr;
    uint32_t converted_addr;

    if (inet_pton(AF_INET, "0.0.0.0", &converted_addr) < 1) {
        perror("inet_pton");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = converted_addr;
    server_addr.sin_port = htons(9000);

    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(sockfd, 16) == -1) {
        perror("listen");
        return -1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char client_addr_str[INET_ADDRSTRLEN] = { 0 };

    for (;;) {
        if ((connfd = accept(sockfd, (struct sockaddr*) &client_addr, &client_addr_len)) < 0) {
            perror("accept");
            return -1;
        }

        if (inet_ntop(AF_INET, &(client_addr.sin_addr), client_addr_str, sizeof(client_addr_str)) == NULL) {
            perror("inet_ntop");
            return -1;
        }

        syslog(LOG_INFO, "Accepted connection from %s\n", client_addr_str);

        data_file = fopen("/var/tmp/aesdsocketdata", "a");

        char buf[1024];
        int read_len;
        while ((read_len = read(connfd, buf, sizeof(buf))) > 0) {
            fwrite(buf, sizeof(char), read_len, data_file);
            if (buf[read_len - 1] == '\n') {
                break;
            }
        }
        if (read_len == -1) {
            perror("read");
            return -1;
        }

        fclose(data_file);
        data_file = fopen("/var/tmp/aesdsocketdata", "r");

        while ((read_len = fread(buf, sizeof(char), 1024, data_file)) > 0) {
            write(connfd, buf, read_len);
        }

        close(connfd);
        connfd = -1;
        fclose(data_file);
        data_file = NULL;
        syslog(LOG_INFO, "Closed connection from %s\n", client_addr_str);
    }

    return 0;
}