#include <zephyr/net/socket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

#define SEND_INTERVAL K_SECONDS(5)

void main(void) {
    const char *device_state = "work";
    int sock;
    struct sockaddr_in server_addr;

    /* Create UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    /* Initialize server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        /* Send device state to server */
        int ret = sendto(sock, device_state, strlen(device_state), 0,
                         (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret < 0) {
            printk("Failed to send data\n");
            /* Optionally handle error */
        } else {
            printk("Data sent successfully\n");
        }

        /* Wait for next interval */
        k_sleep(SEND_INTERVAL);
    }

    /* Close the socket */
    close(sock);
}