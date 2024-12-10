
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_MS 5000

static int send_udp_message(int sock, const char *message)
{
    struct sockaddr_in server_addr;
    int ret;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ret = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("UDP send failed: %d\n", errno);
        return -1;
    }

    return 0;
}

int main(void)
{
    int sock;
    int ret;
    bool device_state = true; // true for "work", false for "not work"

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket: %d\n", errno);
        return -1;
    }

    while (1) {
        const char *state_message = device_state ? DEVICE_STATE_WORK : DEVICE_STATE_NOT_WORK;
        ret = send_udp_message(sock, state_message);
        if (ret < 0) {
            printk("Failed to send UDP message\n");
        } else {
            printk("Sent device state: %s\n", state_message);
        }

        // Toggle device state
        device_state = !device_state;

        // Wait for 5 seconds before sending the next message
        k_sleep(K_MSEC(SEND_INTERVAL_MS));
    }

    // Close the socket (this will never be reached in this example)
    close(sock);

    return 0;
}
