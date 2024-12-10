
#include <zephyr.h>
#include <sys/printk.h>
#include <net/socket.h>
#include <device.h>
#include <drivers/gpio.h>

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

#define GPIO_PIN 21
#define GPIO_FLAGS GPIO_OUTPUT_INIT_LOW

static int setup_udp_socket(void)
{
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_ADDR, &server_addr.sin_addr);

    return sock;
}

static void send_state(int sock, const char *state)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_ADDR, &server_addr.sin_addr);

    sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

void main(void)
{
    const struct device *gpio_dev;
    int sock;
    int ret;
    bool device_state = false;

    gpio_dev = device_get_binding("GPIO_21");
    if (!gpio_dev) {
        printk("Error: didn't find GPIO device\n");
        return;
    }

    ret = gpio_pin_configure(gpio_dev, GPIO_PIN, GPIO_FLAGS);
    if (ret < 0) {
        printk("Error configuring GPIO pin\n");
        return;
    }

    sock = setup_udp_socket();
    if (sock < 0) {
        return;
    }

    while (1) {
        device_state = !device_state;
        if (device_state) {
            gpio_pin_set(gpio_dev, GPIO_PIN, 1);
            send_state(sock, "work");
        } else {
            gpio_pin_set(gpio_dev, GPIO_PIN, 0);
            send_state(sock, "not work");
        }

        k_sleep(SEND_INTERVAL);
    }
}
