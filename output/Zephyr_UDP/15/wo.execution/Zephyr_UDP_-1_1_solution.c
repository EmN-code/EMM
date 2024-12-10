
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

LOG_MODULE_REGISTER(udp_client);

static int connect_wifi(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    return net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
}

static int create_udp_socket(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket: %d", sock);
        return -1;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
        .sin_addr.s_addr = inet_addr(SERVER_IP),
    };

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERR("Failed to connect to server: %d", errno);
        close(sock);
        return -1;
    }

    return sock;
}

static void send_device_state(int sock, const char *state) {
    if (send(sock, state, strlen(state), 0) < 0) {
        LOG_ERR("Failed to send data: %d", errno);
    } else {
        LOG_INF("Sent: %s", state);
    }
}

int main(void) {
    if (connect_wifi() < 0) {
        LOG_ERR("Failed to connect to WiFi");
        return -1;
    }

    int sock = create_udp_socket();
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return -1;
    }

    while (1) {
        send_device_state(sock, "work");
        k_sleep(K_SECONDS(5));
        send_device_state(sock, "not work");
        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}
