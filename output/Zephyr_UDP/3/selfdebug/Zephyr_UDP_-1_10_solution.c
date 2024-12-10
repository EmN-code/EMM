
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"

LOG_MODULE_REGISTER(main);

void send_device_state(int sock, const struct sockaddr_in *server_addr)
{
    const char *state_msg = DEVICE_STATE_MSG;
    ssize_t msg_len = strlen(state_msg);

    if (sendto(sock, state_msg, msg_len, 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) != msg_len) {
        LOG_ERR("Failed to send device state");
    } else {
        LOG_INF("Device state sent: %s", state_msg);
    }
}

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    while (1) {
        send_device_state(sock, &server_addr);
        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}
