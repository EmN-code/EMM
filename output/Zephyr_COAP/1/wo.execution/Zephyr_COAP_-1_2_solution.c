
#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    struct sockaddr_in server_addr;
    int sock;
    char buffer[128];
    int ret;

    // Initialize the WiFi interface
    struct net_if *iface = net_if_get_default();
    if (!iface) {
        printk("Failed to get default network interface\n");
        return -1;
    }

    // Connect to WiFi
    struct wifi_connect_req_params wifi_params = {
        .ssid = "your_wifi_ssid",
        .ssid_length = strlen("your_wifi_ssid"),
        .psk = "your_wifi_password",
        .psk_length = strlen("your_wifi_password"),
        .channel = WIFI_CHANNEL_ANY,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params));
    if (ret) {
        printk("WiFi connection failed: %d\n", ret);
        return -1;
    }

    // Wait for the WiFi connection to be established
    k_sleep(K_SECONDS(10));

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Prepare the CoAP request
    int payload_len = strlen(PAYLOAD);
    int uri_len = strlen(URI_PATH);
    int total_len = 4 + uri_len + payload_len; // CoAP header + URI + Payload

    buffer[0] = 0x40; // CoAP version 1, Type: Confirmable, Token length: 0
    buffer[1] = 0x02; // CoAP Code: POST
    buffer[2] = 0x00; // Message ID: 0 (can be any unique value)
    buffer[3] = 0x00; // Message ID: 0 (can be any unique value)

    // Append URI path option
    buffer[4] = 0xB1; // Option Delta: 11, Length: 1
    buffer[5] = uri_len;
    memcpy(&buffer[6], URI_PATH, uri_len);

    // Append payload marker
    buffer[6 + uri_len] = 0xFF;

    // Append payload
    memcpy(&buffer[7 + uri_len], PAYLOAD, payload_len);

    // Send the CoAP request
    ret = sendto(sock, buffer, total_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
