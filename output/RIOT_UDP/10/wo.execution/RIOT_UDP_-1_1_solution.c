#include <udp.h>
#include <unistd.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/ipv6/addr.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 64

static char udp_buffer[BUFFER_SIZE];

void *udp_sender(void *arg) {
    (void)arg;
    struct sockaddr_in server_addr;
    int sockfd;
    const char *message = "work";

    /* Create UDP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("Failed to create socket\n");
        return NULL;
    }

    /* Set server address parameters */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        /* Send message to server */
        if (sendto(sockfd, message, strlen(message), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        
        /* Wait for 5 seconds before sending the next message */
        sleep(5);
    }

    /* Close the socket */
    close(sockfd);
    return NULL;
}

int main(void) {
    kernel_pid_t udp_thread;
    udp_thread = thread_create(udp_buffer, sizeof(udp_buffer),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               udp_sender, NULL, "udp_sender");

    /* Join the thread to ensure it runs continuously */
    thread_join(udp_thread);

    return 0;
}