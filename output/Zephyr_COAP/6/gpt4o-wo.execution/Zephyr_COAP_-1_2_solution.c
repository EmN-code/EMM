
#include <zephyr/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/net/coap.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct wifi_connect_req_params wifi_params = {
    .ssid = WIFI_SSID,
    .ssid_length = sizeof(WIFI_SSID) - 1,
    .psk = WIFI_PSK,
    .psk_length = sizeof(WIFI_PSK) - 1,
    .channel = 0,
    .security = WIFI_SECURITY_TYPE_PSK,
};

int main(void) {
    int ret;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];

    // Initialize WiFi
    ret = wifi_connect(&wifi_params);
    if (ret) {
        printk("WiFi connection failed: %d\n", ret);
        return ret;
    }
    printk("WiFi connected\n");

    // Create a UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return sock;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP packet
    coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, 0);
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, sizeof(COAP_URI_PATH) - 1);
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, "Hello, COAP Cloud", 17);

    // Send the CoAP request
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request: %d\n", ret);
        close(sock);
        return ret;
    }

    printk("CoAP request sent\n");

    // Close the socket
    close(sock);

    return 0;
}
