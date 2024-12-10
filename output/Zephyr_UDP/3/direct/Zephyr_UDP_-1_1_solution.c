
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
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

static int connect_udp_server(int *sock, struct sockaddr_in *server_addr)
{
    *sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (*sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(UDP_SERVER_PORT);
    if (inet_pton(AF_INET, UDP_SERVER_IP, &server_addr->sin_addr) <= 0) {
        LOG_ERR("Invalid address/ Address not supported");
        return -1;
    }

    return 0;
}

static void send_device_state(int sock, struct sockaddr_in *server_addr)
{
    const char *state = (k_uptime_get() % 10000 < 5000) ? DEVICE_STATE_WORK : DEVICE_STATE_NOT_WORK;
    ssize_t bytes_sent = sendto(sock, state, strlen(state), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (bytes_sent < 0) {
        LOG_ERR("Failed to send data to UDP server");
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;

    if (connect_udp_server(&sock, &server_addr) < 0) {
        return -1;
    }

    while (1) {
        send_device_state(sock, &server_addr);
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    return 0;
}
