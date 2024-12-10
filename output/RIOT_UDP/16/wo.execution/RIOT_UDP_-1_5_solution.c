#include <mutex.h>
#include <arpa/inet.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>
#include <net/ipv6/addr.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <event/thread.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x_params.h>
#include <esp_wifi.h>
#include <esp_system.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5

void *udp_sender(void *arg) {
    int sockfd;
    struct sockaddr_in servaddr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Failed to create socket.\n");
        return NULL;
    }

    // Server details
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
        printf("Invalid address/Address not supported.\n");
        return NULL;
    }

    while (1) {
        const char *state = "work";
        if (sendto(sockfd, state, strlen(state), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            printf("Failed to send data.\n");
        } else {
            printf("State sent: %s\n", state);
        }

        // Wait for the specified interval before sending again
        sleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_t thread;

    // Create a separate thread for sending UDP packets
    thread_create(&thread, NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 
                  THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender");

    // Main thread does nothing, wait forever
    while (1) {
        sleep(1);
    }

    return 0;
}