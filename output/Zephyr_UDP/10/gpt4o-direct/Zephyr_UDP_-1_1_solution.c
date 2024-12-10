
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

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
        printk("Failed to send UDP message\n");
    } else {
        printk("Sent device state: %s\n", state);
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
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Simulate device state (work or not work)
    bool device_working = true;

    while (1) {
        if (device_working) {
            send_device_state(DEVICE_STATE_WORK);
        } else {
            send_device_state(DEVICE_STATE_NOT_WORK);
        }

        // Toggle device state every 5 seconds
        device_working = !device_working;
        k_sleep(K_MSEC(SEND_INTERVAL_MS));
    }
}
