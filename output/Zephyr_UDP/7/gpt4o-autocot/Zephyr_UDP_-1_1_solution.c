
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

LOG_MODULE_REGISTER(main);

const struct device *gpio_dev;

void send_udp_message(int sock, const char *message) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ssize_t ret = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send UDP message: %d", errno);
    } else {
        LOG_INF("Sent UDP message: %s", message);
    }
}

int main(void) {
    int sock;
    struct sockaddr_in local_addr;

    // Initialize GPIO
    gpio_dev = device_get_binding(DT_LABEL(DT_NODELABEL(gpio0)));
    if (!gpio_dev) {
        LOG_ERR("Failed to get GPIO device");
        return -1;
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return -1;
    }

    // Bind socket to local address
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = 0; // Let the system assign a port
    local_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        LOG_ERR("Failed to bind socket: %d", errno);
        close(sock);
        return -1;
    }

    while (1) {
        // Simulate device state: "work" or "not work"
        const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
        send_udp_message(sock, state);

        k_sleep(SEND_INTERVAL);
    }

    close(sock);
    return 0;
}