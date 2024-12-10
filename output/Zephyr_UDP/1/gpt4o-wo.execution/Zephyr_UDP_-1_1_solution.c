#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define MESSAGE_INTERVAL K_SECONDS(5)

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    char *device_state = "Work";

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket, %d\n", errno);
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Send device state to server
        if (sendto(sock, device_state, strlen(device_state), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printk("Failed to send data, %d\n", errno);
        } else {
            printk("Data sent: %s\n", device_state);
        }

        k_sleep(MESSAGE_INTERVAL);

        // Toggle state for simulation
        if (strcmp(device_state, "Work") == 0) {
            device_state = "Not work";
        } else {
            device_state = "Work";
        }
    }

    close(sock);
    printk("UDP connection closed\n");
}