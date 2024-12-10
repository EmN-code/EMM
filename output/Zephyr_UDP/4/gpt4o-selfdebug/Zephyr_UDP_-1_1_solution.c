#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_TIME_MS 5000

const char *device_state[] = {"work", "not work"};

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    int ret;
    int state_idx = 0;

    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket, errno %d\n", errno);
        return;
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Send device state to server
        ret = sendto(sock, device_state[state_idx],
                     strlen(device_state[state_idx]), 0,
                     (struct sockaddr *)&server_addr,
                     sizeof(server_addr));
        if (ret < 0) {
            printk("Failed to send data, errno %d\n", errno);
        } else {
            printk("Sent device state: %s\n", device_state[state_idx]);
        }

        // Toggle device state
        state_idx = 1 - state_idx;

        // Sleep for a while
        k_msleep(SLEEP_TIME_MS);
    }

    // Close socket
    close(sock);
}