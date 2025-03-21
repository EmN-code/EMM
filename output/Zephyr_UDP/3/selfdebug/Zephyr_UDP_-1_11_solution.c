
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"

LOG_MODULE_REGISTER(udp_client);

void send_device_state(int sock, const struct sockaddr_in *server_addr)
{
    const char *state_msg = DEVICE_STATE_MSG;
    ssize_t ret;

    ret = sendto(sock, state_msg, strlen(state_msg), 0,
                 (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send UDP message (%d)", errno);
    } else {
        LOG_INF("Sent device state: %s", state_msg);
    }
}

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket (%d)", errno);
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        send_device_state(sock, &server_addr);
        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}
