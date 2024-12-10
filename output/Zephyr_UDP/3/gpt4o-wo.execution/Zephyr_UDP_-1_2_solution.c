#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    const char *message = "work";
    int ret;

    /* Create a UDP socket */
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    /* Configure server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        /* Send message to the server */
        ret = zsock_sendto(sock, message, strlen(message), 0, 
                  (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            printk("Failed to send message\n");
            break;
        }

        /* Wait until the next send interval */
        k_sleep(SEND_INTERVAL);
    }

    zsock_close(sock);
}