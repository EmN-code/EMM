
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_SECONDS 5

LOG_MODULE_REGISTER(udp_client);

const struct device *gpio_dev;

void send_device_state(int sock, const char *state)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ssize_t ret = sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send UDP message: %d", errno);
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

void main(void)
{
    int sock;
    struct sockaddr_in local_addr;

    // Initialize GPIO device
    gpio_dev = device_get_binding("GPIO_0");
    if (!gpio_dev) {
        LOG_ERR("Failed to get GPIO device");
        return;
    }

    // Configure GPIO pin for input
    gpio_pin_configure(gpio_dev, 13, GPIO_INPUT);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket: %d", errno);
        return;
    }

    // Bind socket to local address
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = 0;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        LOG_ERR("Failed to bind UDP socket: %d", errno);
        close(sock);
        return;
    }

    while (1) {
        // Read GPIO pin state
        int pin_state = gpio_pin_get(gpio_dev, 13);
        const char *device_state = pin_state ? DEVICE_STATE_WORK : DEVICE_STATE_NOT_WORK;

        // Send device state to UDP server
        send_device_state(sock, device_state);

        // Wait for 5 seconds before sending the next state
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    // Close socket (this will never be reached in this example)
    close(sock);
}
