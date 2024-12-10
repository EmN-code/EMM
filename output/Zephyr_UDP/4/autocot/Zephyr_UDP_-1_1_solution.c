
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL_SECONDS 5

static int udp_socket;
static struct sockaddr_in server_addr;

void send_device_state(const char *state)
{
    if (sendto(udp_socket, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("UDP send failed\n");
    } else {
        printk("Sent: %s\n", state);
    }
}

void main(void)
{
    int ret;
    struct sockaddr_in local_addr;

    // Create UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    // Configure local address
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = 0; // Let the system assign a random port

    // Bind the socket to the local address
    ret = bind(udp_socket, (struct sockaddr *)&local_addr, sizeof(local_addr));
    if (ret < 0) {
        printk("Failed to bind UDP socket\n");
        close(udp_socket);
        return;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    printk("UDP client started\n");

    while (1) {
        // Simulate device state
        const char *state = "work"; // or "not work"
        send_device_state(state);

        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }
}
