
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/printk.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void send_coap_request(int sock, struct sockaddr_in *server_addr) {
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    // Initialize CoAP packet
    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, 0, 0, COAP_METHOD_POST, 0);
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        return;
    }

    // Set URI path
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        printk("Failed to append URI path option\n");
        return;
    }

    // Append payload
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker\n");
        return;
    }

    ret = coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        printk("Failed to append payload\n");
        return;
    }

    // Send CoAP request
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        return;
    }

    printk("CoAP request sent successfully\n");
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    // Initialize WiFi
    struct net_if *iface = net_if_get_default();
    if (!iface) {
        printk("Failed to get default network interface\n");
        return -1;
    }

    // Connect to WiFi (assuming WiFi credentials are already set up)
    wifi_connect(iface, NULL, NULL);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Send CoAP request
    send_coap_request(sock, &server_addr);

    // Close socket
    close(sock);

    return 0;
}
