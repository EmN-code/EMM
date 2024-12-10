
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SLEEP_TIME_MS 5000

LOG_MODULE_REGISTER(udp_client);

void main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket: %d", errno);
        return;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    ret = inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);
    if (ret < 0) {
        LOG_ERR("Invalid address: %d", errno);
        close(sock);
        return;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        // Send the device state message
        ret = sendto(sock, DEVICE_STATE_MSG, sizeof(DEVICE_STATE_MSG), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            LOG_ERR("Failed to send data: %d", errno);
        } else {
            LOG_INF("Sent device state: %s", DEVICE_STATE_MSG);
        }

        // Sleep for 5 seconds
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    // Close the socket
    close(sock);
}
