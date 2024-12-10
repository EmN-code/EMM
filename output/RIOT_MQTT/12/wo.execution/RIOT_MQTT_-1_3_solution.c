#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <event/thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

void *publish_thread(void *arg) {
    (void)arg;
    int sock;
    struct sockaddr_in server_addr;
    sock_udp_ep_t server;

    // Setup UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        return NULL;
    }

    // Setup server address
    server.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    server.port = SERVER_PORT;

    // Message publishing loop
    char *message = "work";
    while (1) {
        int sent = sock_udp_send(NULL, message, strlen(message), &server);
        if (sent < 0) {
            perror("sendto");
        }
        // Wait for 5 seconds before next publish
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish");
    return 0;
}