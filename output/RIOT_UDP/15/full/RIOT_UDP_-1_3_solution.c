#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "netinet/in.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_INTERVAL 5

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    int res;

    // Set server address
    if (inet_pton(AF_INET6, SERVER_IP, &server.addr) != 1) {
        printf("ERROR: unable to parse IPv6 address\n");
        return 1;
    }

    // Create UDP socket
    while (1) {
        res = sock_udp_create(&sock, NULL, &server, 0);
        if (res == 0) {
            printf("Socket created successfully\n");
            break;
        } else {
            printf("Failed to create socket, trying again...\n");
            sock_udp_close(&sock);
            sleep(1);
        }
    }

    // Message to send
    char *msg = "device state: work";

    // Send message every MSG_INTERVAL seconds
    while (1) {
        ssize_t len = sock_udp_send(&sock, msg, strlen(msg), &server);
        if (len < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %s\n", msg);
        }

        sleep(MSG_INTERVAL);
    }

    return 0;
}