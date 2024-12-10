
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_MS 5000

LOG_MODULE_REGISTER(udp_client);

static int connect_to_udp_server(int *sock)
{
    struct sockaddr_in server_addr;

    *sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (*sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    return 0;
}

static void send_device_state(int sock, const char *state)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ssize_t bytes_sent = sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent < 0) {
        LOG_ERR("Failed to send data to UDP server");
    } else {
        LOG_INF("Sent %d bytes: %s", (int)bytes_sent, state);
    }
}

int main(void)
{
    int sock;
    int device_state = 1; // 1 for work, 0 for not work

    if (connect_to_udp_server(&sock) < 0) {
        LOG_ERR("Failed to connect to UDP server");
        return -1;
    }

    while (1) {
        if (device_state) {
            send_device_state(sock, DEVICE_STATE_WORK);
        } else {
            send_device_state(sock, DEVICE_STATE_NOT_WORK);
        }

        device_state = !device_state; // Toggle state

        k_sleep(K_MSEC(SEND_INTERVAL_MS));
    }

    close(sock);
    return 0;
}
