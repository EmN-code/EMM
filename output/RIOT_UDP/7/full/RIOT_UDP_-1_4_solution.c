#include "mqtt_params.h" // Custom header for any mqtt parameters you might have
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "thread.h"
#include "netinet/in.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 128

static sock_udp_t sock;
static sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

int main(void) {
    // Converting the IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address.");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket.");
        return 1;
    }

    char *message = "work";

    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);

        if (res < 0) {
            printf("Error sending packet: %d\n", res);
        } else if (res != strlen(message)) {
            printf("Error: not all data sent. Sent %d of %d bytes\n", res, (int)strlen(message));
        } else {
            printf("Message sent successfully!\n");
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}