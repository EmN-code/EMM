
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_NOT_WORK_MSG "not work"
#define SEND_INTERVAL_SECONDS 5

LOG_MODULE_REGISTER(udp_client);

static int create_udp_socket(void)
{
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERR("Failed to connect to UDP server");
        close(sock);
        return -1;
    }

    return sock;
}

static void send_device_state(int sock, const char *state)
{
    ssize_t len;

    len = send(sock, state, strlen(state), 0);
    if (len < 0) {
        LOG_ERR("Failed to send device state");
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

int main(void)
{
    int sock;
    bool device_working = true; // Simulate device state

    LOG_INF("Starting UDP client");

    sock = create_udp_socket();
    if (sock < 0) {
        return -1;
    }

    while (1) {
        if (device_working) {
            send_device_state(sock, DEVICE_STATE_MSG);
        } else {
            send_device_state(sock, DEVICE_NOT_WORK_MSG);
        }

        device_working = !device_working; // Toggle device state

        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    close(sock);
    return 0;
}
