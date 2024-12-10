
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"

void send_udp_message(int sock, const char *message) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    ssize_t ret = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("UDP message send failed: %d\n", errno);
    } else {
        printk("UDP message sent: %s\n", message);
    }
}

int main(void) {
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket: %d\n", errno);
        return -1;
    }

    while (1) {
        send_udp_message(sock, DEVICE_STATE);
        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}
