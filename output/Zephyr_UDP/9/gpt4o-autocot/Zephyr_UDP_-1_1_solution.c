
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL 5000  // 5 seconds

LOG_MODULE_REGISTER(main);

const struct device *gpio_dev;

void send_udp_message(const char *message) {
    int sock;
    struct sockaddr_in server_addr;
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return;
    }

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
}

void main(void) {
    gpio_dev = device_get_binding(DT_LABEL(DT_NODELABEL(gpio0)));
    if (!gpio_dev) {
        LOG_ERR("Failed to get GPIO device");
        return;
    }

    while (1) {
        // Simulate device state: "work" or "not work"
        const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
        send_udp_message(state);
        k_sleep(K_MSEC(SEND_INTERVAL));
    }
}
