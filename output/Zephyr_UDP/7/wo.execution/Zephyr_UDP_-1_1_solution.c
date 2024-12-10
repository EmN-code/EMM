
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL K_SECONDS(5)

LOG_MODULE_REGISTER(udp_client);

static int connect_udp_server(struct sockaddr_in *server_addr)
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr->sin_addr);

    return sock;
}

static void send_device_state(int sock, struct sockaddr_in *server_addr, const char *state)
{
    ssize_t ret;

    ret = sendto(sock, state, strlen(state), 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send device state");
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

int main(void)
{
    struct sockaddr_in server_addr;
    int sock;
    bool device_working = true;

    LOG_INF("Starting UDP client");

    sock = connect_udp_server(&server_addr);
    if (sock < 0) {
        return -1;
    }

    while (1) {
        if (device_working) {
            send_device_state(sock, &server_addr, DEVICE_STATE_WORK);
        } else {
            send_device_state(sock, &server_addr, DEVICE_STATE_NOT_WORK);
        }

        device_working = !device_working; // Simulate device state change
        k_sleep(SEND_INTERVAL);
    }

    return 0;
}
