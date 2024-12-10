
#include <stdio.h>
#include <string.h>
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void udp_rx_callback(void *arg, int src_addr, uint16_t src_port, const uint8_t *data, uint16_t data_len)
{
    // Optional: Handle incoming UDP packets if needed
    (void)arg;
    (void)src_addr;
    (void)src_port;
    (void)data;
    (void)data_len;
}

int main(void)
{
    // Initialize UDP client
    simple_udp_register(SERVER_PORT, udp_rx_callback, NULL);

    // Prepare the message to send
    char message[16];
    snprintf(message, sizeof(message), "%s", DEVICE_STATE);

    while (1) {
        // Send the message to the server
        simple_udp_sendto(SERVER_IP, SERVER_PORT, (uint8_t *)message, strlen(message));

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
