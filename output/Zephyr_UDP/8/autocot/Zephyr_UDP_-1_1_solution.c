
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL_SECONDS 5

LOG_MODULE_REGISTER(main);

const struct device *gpio_dev;

void send_udp_message(int sock, const char *message) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ssize_t bytes_sent = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent < 0) {
        LOG_ERR("Failed to send UDP message: %d", errno);
    } else {
        LOG_INF("Sent %d bytes: %s", (int)bytes_sent, message);
    }
}

void main(void) {
    int sock;
    struct sockaddr_in local_addr;

    // Initialize GPIO
    gpio_dev = device_get_binding(DT_LABEL(DT_NODELABEL(gpio0)));
    if (!gpio_dev) {
        LOG_ERR("Failed to get GPIO device");
        return;
    }

    // Configure GPIO pin for input
    gpio_pin_configure(gpio_dev, DT_GPIO_PIN(DT_NODELABEL(gpio0), gpios), GPIO_INPUT);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return;
    }

    // Bind socket to local address
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = 0;

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        LOG_ERR("Failed to bind socket: %d", errno);
        close(sock);
        return;
    }

    while (1) {
        // Read GPIO state
        int state = gpio_pin_get(gpio_dev, DT_GPIO_PIN(DT_NODELABEL(gpio0), gpios));
        const char *message = state ? "work" : "not work";

        // Send UDP message
        send_udp_message(sock, message);

        // Wait for 5 seconds
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    // Close socket (this will never be reached in this example)
    close(sock);
}
