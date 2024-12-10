
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

LOG_MODULE_REGISTER(udp_client);

static int udp_socket;
static struct sockaddr_in server_addr;

void send_device_state(const char *state)
{
    if (sendto(udp_socket, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERR("Failed to send UDP message");
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

void main(void)
{
    int ret;

    // Initialize the UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket < 0) {
        LOG_ERR("Failed to create UDP socket");
        return;
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Simulate device state
    const char *states[] = {"work", "not work"};
    int state_index = 0;

    while (1) {
        send_device_state(states[state_index]);
        state_index = (state_index + 1) % 2; // Toggle between "work" and "not work"
        k_sleep(SEND_INTERVAL);
    }
}
