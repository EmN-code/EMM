#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>

#define SERVER_PORT 20001
#define SERVER_ADDR "47.102.103.1"
#define DEVICE_STATE "work"
#define SLEEP_TIME_MS 5000

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    int ret;

    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        ret = zsock_sendto(sock, DEVICE_STATE, sizeof(DEVICE_STATE), 0,
                           (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret < 0) {
            printk("Failed to send data\n");
            break;
        } else {
            printk("Sent: %s\n", DEVICE_STATE);
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    zsock_close(sock);
}