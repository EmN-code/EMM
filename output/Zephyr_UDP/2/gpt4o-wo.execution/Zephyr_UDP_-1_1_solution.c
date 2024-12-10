#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    const char *device_status = "work or not work";

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        int sent_bytes = sendto(sock, device_status, strlen(device_status), 0,
                                (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_bytes < 0) {
            printk("Failed to send data\n");
        } else {
            printk("Data sent successfully\n");
        }
        k_sleep(SEND_INTERVAL);
    }

    close(sock);
}