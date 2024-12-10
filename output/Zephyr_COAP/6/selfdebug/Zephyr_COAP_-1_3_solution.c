
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/ethernet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static struct sockaddr_in server_addr;

void send_coap_request(void) {
    int sock;
    struct coap_packet request;
    struct coap_option options[1];
    uint8_t buffer[128];

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, 0, COAP_METHOD_POST, coap_next_token());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));

    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
}

int main(void) {
    printk("Starting CoAP client on ESP32\n");

    // Initialize network stack and connect to WiFi
    struct net_if *iface = net_if_get_default();
    if (iface == NULL) {
        printk("Failed to get default network interface\n");
        return -1;
    }

    // Assuming WiFi is already configured and connected
    // You may need to add WiFi connection logic here

    send_coap_request();

    return 0;
}
