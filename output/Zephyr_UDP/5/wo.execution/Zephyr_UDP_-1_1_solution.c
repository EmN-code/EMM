
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

void send_device_state(int sock, const struct sockaddr *server_addr) {
    ssize_t ret;
    ret = sendto(sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, 0, server_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        printk("Failed to send device state: %d\n", errno);
    } else {
        printk("Device state sent successfully\n");
    }
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return -1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Send device state every 5 seconds
    while (1) {
        send_device_state(sock, (struct sockaddr *)&server_addr);
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    // Close socket (this part will never be reached in this example)
    close(sock);

    return 0;
}
