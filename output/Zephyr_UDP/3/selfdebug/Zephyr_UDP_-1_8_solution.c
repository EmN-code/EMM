
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"

LOG_MODULE_REGISTER(main);

void send_device_state(int sock, const char *state) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ssize_t ret = sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send device state: %d", errno);
    } else {
        LOG_INF("Device state sent: %s", state);
    }
}

void main(void) {
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return;
    }

    while (1) {
        send_device_state(sock, DEVICE_STATE);
        k_sleep(K_SECONDS(5));
    }

    close(sock);
}
