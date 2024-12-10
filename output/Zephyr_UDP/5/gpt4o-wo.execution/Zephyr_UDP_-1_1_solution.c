#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define SLEEP_TIME_MS 5000

void main(void) {
    int sock;
    struct sockaddr_in server_addr;

    printk("Starting UDP client\n");

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        ssize_t sent = sendto(sock, MESSAGE, strlen(MESSAGE), 0,
                              (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            printk("Failed to send data: %d\n", errno);
        } else {
            printk("Sent message: %s\n", MESSAGE);
        }
        k_msleep(SLEEP_TIME_MS);
    }

    close(sock);
}