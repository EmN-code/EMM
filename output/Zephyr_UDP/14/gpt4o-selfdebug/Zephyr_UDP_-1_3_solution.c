#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(udp_client, LOG_LEVEL_DBG);

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SLEEP_TIME K_SECONDS(5)

void main(void)
{
    struct sockaddr_in server_addr;
    int sock;
    char device_state[] = "work";

    /* Create a UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return;
    }

    /* Server address configuration */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    while (1) {
        /* Send device state to server */
        int ret = sendto(sock, device_state, sizeof(device_state), 0,
                         (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret < 0) {
            LOG_ERR("Failed to send UDP packet");
        } else {
            LOG_DBG("Device state sent: %s", device_state);
        }

        /* Sleep for specified interval */
        k_sleep(SLEEP_TIME);
    }

    /* Close the socket (not reachable in current logic) */
    close(sock);
}