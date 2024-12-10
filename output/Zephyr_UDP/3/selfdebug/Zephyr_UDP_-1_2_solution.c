
#include <zephyr.h>
#include <net/socket.h>
#include <device.h>
#include <drivers/gpio.h>
#include <logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

#define GPIO_PIN 13  // Example GPIO pin, adjust according to your setup

LOG_MODULE_REGISTER(udp_client);

void send_state(int sock, const struct sockaddr *server_addr)
{
    const char *state = gpio_pin_get(device_get_binding("GPIO_0"), GPIO_PIN) ? "work" : "not work";
    sendto(sock, state, strlen(state), 0, server_addr, sizeof(struct sockaddr_in));
    LOG_INF("Sent state: %s", state);
}

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    struct device *gpio_dev;

    // Initialize GPIO
    gpio_dev = device_get_binding("GPIO_0");
    if (!gpio_dev) {
        LOG_ERR("Cannot find GPIO device");
        return -1;
    }

    gpio_pin_configure(gpio_dev, GPIO_PIN, GPIO_INPUT);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Send device state every 5 seconds
    while (1) {
        send_state(sock, (struct sockaddr *)&server_addr);
        k_sleep(SEND_INTERVAL);
    }

    return 0;
}
