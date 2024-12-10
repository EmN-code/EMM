
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_MS 5000

static int udp_socket;
static struct sockaddr_in server_addr;

void send_device_state(const char *state)
{
    if (sendto(udp_socket, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("UDP send failed\n");
    } else {
        printk("Sent device state: %s\n", state);
    }
}

void main(void)
{
    int ret;
    struct timeval timeout;

    // Create UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    // Set socket timeout
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(udp_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Simulate device state
    bool device_working = true;

    while (1) {
        if (device_working) {
            send_device_state(DEVICE_STATE_WORK);
        } else {
            send_device_state(DEVICE_STATE_NOT_WORK);
        }

        // Toggle device state
        device_working = !device_working;

        // Wait for 5 seconds
        k_sleep(K_MSEC(SEND_INTERVAL_MS));
    }
}
