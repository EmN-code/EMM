#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define SERVER_HOST "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    const char *device_state = "work";

    /* Create a UDP socket */
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket. Error: %d\n", errno);
        return;
    }

    /* Configure the server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_HOST, &server_addr.sin_addr);

    while (1) {
        /* Send the device state to the server */
        ssize_t sent = zsock_sendto(sock, device_state, strlen(device_state), 0,
                                    (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            printk("Failed to send data. Error: %d\n", errno);
        } else {
            printk("Sent %d bytes to server\n", sent);
        }

        /* Wait for the next send interval */
        k_sleep(SEND_INTERVAL);
    }

    /* Close the socket */
    zsock_close(sock);
}