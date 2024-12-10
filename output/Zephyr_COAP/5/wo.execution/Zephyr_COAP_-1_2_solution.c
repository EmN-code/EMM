
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/coap.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];

    // Initialize the socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP packet
    coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());

    // Add the URI path option
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));

    // Add the payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));

    // Send the CoAP request
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
