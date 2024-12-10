#include "mutex.h"
#include "netinet/in.h"
#include "dht_params.h"
#include "net/emcute.h"
#include "esp_system.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/af.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_INTERVAL 5

typedef struct sockaddr_in6 sockaddr_in6_t;

typedef struct in6_addr in6_addr_t;

static void *udp_sender(void *arg) {
    (void)arg;

    int sock;
    struct sockaddr_in6 server_addr;
    char message[12]; // "work" or "not work"
    "work"

    // Create UDP socket
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Error: socket\n");
        return NULL;
    }

    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;

    server_addr.sin6_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin6_addr) <= 0) {
        printf("Error: invalid IP address\n");
        return NULL;
    }

    // Enter the send loop
    while (1) {
        int message_length = snprintf(message, sizeof(message), "work");
        if (message_length < 0 || message_length >= sizeof(message)) {
            printf("Error: message truncation\n");
            continue; 
        }

        if (sendto(sock, message, message_length, 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printf("Error: sendto\n");
        }

        xtimer_sleep(MSG_INTERVAL);
    }

    close(sock);
    return NULL;
}

int main(void) {
    printf("UDP client example with ESP32\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender");

    return 0;
}
