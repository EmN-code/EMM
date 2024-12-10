#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL_MS 5000

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    int ret;
    const char *message = "device state: work";

    /* Create a UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    /* Initialize server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        /* Send the message to server */
        ret = sendto(sock, message, strlen(message), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret < 0) {
            printk("Failed to send message to server\n");
        } else {
            printk("Message sent\n");
        }

        /* Wait for the defined send interval */
        k_msleep(SEND_INTERVAL_MS);
    }

    /* Close the socket */
    close(sock);
}