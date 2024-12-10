
#include <stdio.h>
#include <xtimer.h>
#include <net/ipv6/simple-udp.h>
#include <net/ipv6/uip-udp-packet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work" // or "not work"
#define INTERVAL_MS 5000000

static void udp_rx_cb(void *arg, int src_addr, uint16_t src_port, const uint8_t *data, uint16_t len)
{
    (void)arg;
    (void)src_addr;
    (void)src_port;
    (void)data;
    (void)len;
    // Optional: Handle incoming UDP packets if needed
}

int main(void)
{
    // Initialize UDP connection
    simple_udp_register(SERVER_PORT, udp_rx_cb, NULL);

    // Main loop
    while (1) {
        // Send device state to the server
        simple_udp_sendto(SERVER_IP, SERVER_PORT, (uint8_t *)DEVICE_STATE, sizeof(DEVICE_STATE));

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
