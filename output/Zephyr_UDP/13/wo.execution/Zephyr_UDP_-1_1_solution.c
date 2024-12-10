
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_SECONDS 5

LOG_MODULE_REGISTER(udp_client);

static int udp_socket;
static struct sockaddr_in udp_server_addr;

void send_device_state(const char *state)
{
    if (sendto(udp_socket, state, strlen(state), 0, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr)) < 0) {
        LOG_ERR("Failed to send device state");
    } else {
        LOG_INF("Device state sent: %s", state);
    }
}

void main(void)
{
    int ret;
    struct net_if *iface;

    // Initialize the network interface
    iface = net_if_get_default();
    if (!iface) {
        LOG_ERR("Failed to get default network interface");
        return;
    }

    // Initialize the UDP socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket < 0) {
        LOG_ERR("Failed to create UDP socket");
        return;
    }

    // Set up the UDP server address
    memset(&udp_server_addr, 0, sizeof(udp_server_addr));
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_SERVER_PORT);
    ret = inet_pton(AF_INET, UDP_SERVER_IP, &udp_server_addr.sin_addr);
    if (ret < 0) {
        LOG_ERR("Invalid IP address");
        close(udp_socket);
        return;
    }

    // Send device state every 5 seconds
    while (1) {
        send_device_state(DEVICE_STATE_WORK);
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }
}
