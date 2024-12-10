#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/tls_credentials.h>

#include <zephyr.h>
#include <net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define UDP_BUFFER_SIZE 16

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[UDP_BUFFER_SIZE];
    int ret;

    printk("Starting UDP client\n");

    /* Create udp socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket, error: %d\n", errno);
        return;
    }

    /* Initialize server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    ret = inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);
    if (ret <= 0) {
        printk("Invalid server IP address\n");
        close(sock);
        return;
    }

    while (1) {
        /* Determine the device state */
        bool device_working = true; // This is a placeholder. Replace with actual check.
        if (device_working) {
            strcpy(buffer, "work");
        } else {
            strcpy(buffer, "not work");
        }

        /* Send device state */
        ret = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            printk("Failed to send data, error: %d\n", errno);
        } else {
            printk("Sent device state: %s\n", buffer);
        }

        /* Wait 5 seconds */
        k_sleep(K_SECONDS(5));
    }

    close(sock);
}