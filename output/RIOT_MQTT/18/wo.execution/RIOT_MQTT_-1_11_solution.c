#include <xtimer.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <sys/socket.h> 

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "esp32/state"
#define MESSAGE "work"
#define NOT_WORK_MESSAGE "not work"
#define INTERVAL (5U * US_PER_SEC) // send every five seconds
#define BUFFER_SIZE 1024

int main(void) {
    struct sockaddr_in broker_addr;
    int sock;
    char buffer[BUFFER_SIZE];
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    broker_addr.sin_addr.s_addr = inet_addr(BROKER_IP);

    while (1) {
        snprintf(buffer, BUFFER_SIZE, "{\"topic\":\"%s\",\"message\":\"%s\"}", TOPIC, MESSAGE);

        ssize_t sent = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&broker_addr, sizeof(broker_addr));
        if (sent < 0) {
            perror("sendto failed");
        }

        xtimer_usleep(INTERVAL);
    }

    close(sock);
    return 0;
}
