
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL K_SECONDS(5)

LOG_MODULE_REGISTER(udp_client);

static int setup_udp_socket(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERR("Failed to connect to server");
        close(sock);
        return -1;
    }

    return sock;
}

static void send_device_state(int sock, const char *state) {
    ssize_t ret;

    ret = send(sock, state, strlen(state), 0);
    if (ret < 0) {
        LOG_ERR("Failed to send data");
    } else {
        LOG_INF("Sent device state: %s", state);
    }
}

int main(void) {
    int sock;
    bool device_working = true;

    sock = setup_udp_socket();
    if (sock < 0) {
        return -1;
    }

    while (1) {
        if (device_working) {
            send_device_state(sock, DEVICE_STATE_WORK);
        } else {
            send_device_state(sock, DEVICE_STATE_NOT_WORK);
        }

        device_working = !device_working; // Simulate device state change
        k_sleep(SEND_INTERVAL);
    }

    close(sock);
    return 0;
}
