
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_SECONDS 5

LOG_MODULE_REGISTER(udp_client);

static int send_udp_message(const char *message)
{
    int sock;
    struct sockaddr_in server_addr;
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ret = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send UDP message");
    } else {
        LOG_INF("Sent UDP message: %s", message);
    }

    close(sock);
    return ret;
}

void main(void)
{
    int device_state = 1; // 1 for "work", 0 for "not work"

    while (1) {
        if (device_state) {
            send_udp_message(DEVICE_STATE_WORK);
        } else {
            send_udp_message(DEVICE_STATE_NOT_WORK);
        }

        device_state = !device_state; // Toggle state
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }
}
