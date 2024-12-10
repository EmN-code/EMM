#include <zephyr.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(udp_client, LOG_LEVEL_INF);

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 128
#define SLEEP_TIME K_SECONDS(5)

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        strcpy(buffer, "work or not work");
        ret = sendto(sock, buffer, strlen(buffer), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret < 0) {
            LOG_ERR("Failed to send UDP message");
        } else {
            LOG_INF("Sent UDP message: %s", log_strdup(buffer));
        }

        k_sleep(SLEEP_TIME);
    }

    close(sock);
}