#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(udp_sample, LOG_LEVEL_INF);

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define SLEEP_TIME_MS 5000

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        ret = sendto(sock, MESSAGE, strlen(MESSAGE), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret < 0) {
            LOG_ERR("Failed to send message: %d", errno);
        } else {
            LOG_INF("Message sent to %s:%d", SERVER_ADDR, SERVER_PORT);
        }
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    close(sock);
}