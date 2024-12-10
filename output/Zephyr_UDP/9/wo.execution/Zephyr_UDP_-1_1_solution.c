
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

static int udp_socket;
static struct sockaddr_in server_addr;

void udp_send_state(const char *state)
{
    if (sendto(udp_socket, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("UDP send failed\n");
    } else {
        printk("UDP send success: %s\n", state);
    }
}

void main(void)
{
    int ret;

    // Initialize the UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Main loop to send device state every 5 seconds
    while (1) {
        // Simulate device state
        const char *state = "work"; // or "not work"

        udp_send_state(state);

        k_sleep(SEND_INTERVAL);
    }
}
