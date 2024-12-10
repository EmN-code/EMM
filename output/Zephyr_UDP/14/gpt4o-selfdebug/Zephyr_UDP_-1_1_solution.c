#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define INTERVAL K_SECONDS(5)

void main(void) {
    struct sockaddr_in server_addr;
    int sock, ret;
    char state[] = "work or not work";

    LOG_INF("Starting UDP client");

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        ret = sendto(sock, state, sizeof(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            LOG_ERR("Failed to send data: %d", ret);
        } else {
            LOG_INF("Data sent: %s", state);
        }

        k_sleep(INTERVAL);
    }

    close(sock);
}