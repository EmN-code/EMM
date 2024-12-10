
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define SEND_INTERVAL_SECONDS 5

static int udp_socket;
static struct sockaddr_in udp_server_addr;

void udp_send_state(const char *state)
{
    if (sendto(udp_socket, state, DEVICE_STATE_MSG_LEN, 0, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr)) < 0) {
        printk("UDP send failed\n");
    } else {
        printk("UDP send success: %s\n", state);
    }
}

void main(void)
{
    int ret;

    // Create UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    // Set up server address
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &udp_server_addr.sin_addr);

    // Main loop to send device state every 5 seconds
    while (1) {
        udp_send_state(DEVICE_STATE_MSG);
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }
}
