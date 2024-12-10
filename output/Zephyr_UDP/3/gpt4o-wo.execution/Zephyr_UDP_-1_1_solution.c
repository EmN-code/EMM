#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(udp_client, LOG_LEVEL_DBG);

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_TIME_MS 5000
#define DEVICE_STATE "work"

void main(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket, error: %d", errno);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        int ret = sendto(sock, DEVICE_STATE, sizeof(DEVICE_STATE), 0,
                         (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            LOG_ERR("Failed to send UDP packet, error: %d", errno);
        } else {
            LOG_DBG("Sent UDP packet: %s", DEVICE_STATE);
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    close(sock);
}