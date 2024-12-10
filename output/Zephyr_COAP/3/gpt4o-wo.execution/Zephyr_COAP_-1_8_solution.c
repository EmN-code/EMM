#include <zephyr/syscalls/socket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_mgmt.h>
#include <time.h>

int main() {
    struct sockaddr_in server_addr;
    int sock;
    char message[] = "Hello, COAP Cloud";
    ssize_t sent;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    sent = sendto(sock, message, sizeof(message), 0,
                  (const struct sockaddr *)&server_addr, sizeof(server_addr));

    if (sent < 0) {
        printk("Failed to send message\n");
        close(sock);
        return -1;
    }

    printk("Message sent\n");
    close(sock);
    return 0;
}