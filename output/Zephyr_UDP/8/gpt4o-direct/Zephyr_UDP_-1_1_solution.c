
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL 5000  // 5 seconds

LOG_MODULE_REGISTER(udp_client);

void send_udp_message(int sock, struct sockaddr_in *server_addr) {
    const char *message = DEVICE_STATE;
    int message_len = strlen(message);

    int ret = sendto(sock, message, message_len, 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send UDP message: %d", ret);
    } else {
        LOG_INF("Sent UDP message: %s", message);
    }
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return -1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Send UDP message every 5 seconds
    while (1) {
        send_udp_message(sock, &server_addr);
        k_sleep(K_MSEC(SEND_INTERVAL));
    }

    // Close socket (this part will never be reached in this example)
    close(sock);

    return 0;
}
