#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL K_SECONDS(5)

void udp_client_thread(void) {
    struct sockaddr_in server_addr;
    int sock;
    int ret;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    while (1) {
        ret = sendto(sock, MESSAGE, strlen(MESSAGE), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            printk("Failed to send message\n");
        } else {
            printk("Message sent: %s\n", MESSAGE);
        }

        k_sleep(INTERVAL);
    }

    close(sock);
}

void main(void) {
    printk("Starting UDP client sample\n");
    udp_client_thread();
}