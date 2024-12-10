
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
    struct wifi_connect_req_params params = {0};

    params.ssid = WIFI_SSID;
    params.ssid_length = strlen(WIFI_SSID);
    params.psk = WIFI_PSK;
    params.psk_length = strlen(WIFI_PSK);
    params.channel = WIFI_CHANNEL_ANY;
    params.security = WIFI_SECURITY_TYPE_PSK;

    return wifi_connect(iface, &params);
}

static int send_udp_message(int sock, const char *message) {
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
    };

    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    return sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

int main(void) {
    int ret;
    int sock;

    LOG_INF("Starting UDP client...");

    // Connect to Wi-Fi
    ret = connect_wifi();
    if (ret < 0) {
        LOG_ERR("Failed to connect to Wi-Fi: %d", ret);
        return ret;
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return sock;
    }

    // Send device state every 5 seconds
    while (1) {
        const char *message = "work"; // or "not work" based on device state
        ret = send_udp_message(sock, message);
        if (ret < 0) {
            LOG_ERR("Failed to send UDP message: %d", ret);
        } else {
            LOG_INF("Sent UDP message: %s", message);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
