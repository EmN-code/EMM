
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"

LOG_MODULE_REGISTER(main);

void send_device_state(int sock, const struct sockaddr *server_addr) {
    const char *state = DEVICE_STATE;
    sendto(sock, state, strlen(state), 0, server_addr, sizeof(struct sockaddr_in));
    LOG_INF("Sent device state: %s", state);
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    // Initialize the network
    // This part depends on your network setup, e.g., using Wi-Fi or Ethernet
    // For example, if using Wi-Fi:
    // wifi_connect("your_ssid", "your_password", WIFI_MODE_STA);

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return -1;
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Send device state every 5 seconds
    while (1) {
        send_device_state(sock, (struct sockaddr *)&server_addr);
        k_sleep(K_SECONDS(5));
    }

    // Close the socket (this part is unreachable in this example)
    close(sock);

    return 0;
}
