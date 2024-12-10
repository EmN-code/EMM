#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    /* Define and allocate memory for variables */
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buffer[64];
    const char *device_state = "work";

    /* Convert IP address from string to byte representation */
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    /* Infinite loop to send the state every 5 seconds */
    while (1) {
        /* Prepare the buffer and copy the device state into it */
        strcpy((char *)buffer, device_state);

        /* Send the UDP packet */
        if (sock_udp_send(NULL, buffer, strlen((char *)buffer), &server) < 0) {
            puts("Error sending UDP data");
        } else {
            puts("Device state sent");
        }

        /* Wait for the defined interval */
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}