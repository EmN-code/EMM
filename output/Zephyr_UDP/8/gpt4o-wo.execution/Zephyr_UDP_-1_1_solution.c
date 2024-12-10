
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define SEND_INTERVAL_SECONDS 5

LOG_MODULE_REGISTER(main);

void send_device_state(int sock) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    ssize_t ret = sendto(sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send device state: %d", errno);
    } else {
        LOG_INF("Device state sent: %s", DEVICE_STATE_MSG);
    }
}

int main(void) {
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return -1;
    }

    while (1) {
        send_device_state(sock);
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    close(sock);
    return 0;
}
