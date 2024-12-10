
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL 5000  // 5 seconds in milliseconds

LOG_MODULE_REGISTER(udp_client);

void send_state(int sock) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    const char *state = DEVICE_STATE;
    ssize_t ret = sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send data to server");
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

int main(void) {
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    while (1) {
        send_state(sock);
        k_sleep(K_MSEC(SEND_INTERVAL));
    }

    close(sock);
    return 0;
}
