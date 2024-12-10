#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MESSAGE_INTERVAL 5  // send message every 5 seconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_ADDR_ANY;
    char *message = "work or not work";
    uint8_t buf[128];

    // Initialize the socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert server IP string to byte representation
    inet_pton(AF_INET, SERVER_IP, &(server.addr.ipv4));

    while (1) {
        // Send the message to the MQTT broker
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            puts("Error sending message");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for the defined interval before sending the next message
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    // Should never be reached in this example
    sock_udp_close(&sock);
    return 0;
}
