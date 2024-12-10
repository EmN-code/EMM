
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
static int sock;

void send_device_state(const char *state)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    int ret = sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send UDP message: %d", errno);
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

void main(void)
{
    // Initialize GPIO
    gpio_dev = device_get_binding("GPIO_0");
    if (!gpio_dev) {
        LOG_ERR("Could not get GPIO device");
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

    while (1) {
        // Read GPIO pin state
        int pin_state = gpio_pin_get(gpio_dev, 13);
        if (pin_state < 0) {
            LOG_ERR("Failed to read GPIO pin state: %d", pin_state);
        } else {
            if (pin_state == 0) {
                send_device_state("not work");
            } else {
                send_device_state("work");
            }
        }

        // Wait for 5 seconds before sending the next state
        k_sleep(SEND_INTERVAL);
    }
}
