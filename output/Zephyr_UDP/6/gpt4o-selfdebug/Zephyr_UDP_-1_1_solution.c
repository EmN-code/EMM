#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(udp_client, LOG_LEVEL_INF);

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_TIME_MS 5000

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    char *message = "work or not work";

    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        int sent_len = zsock_sendto(sock, message, strlen(message), 0,
                                    (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_len < 0) {
            LOG_ERR("Failed to send message: %d", errno);
        } else {
            LOG_INF("Message sent: %s", message);
        }
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    zsock_close(sock);
}