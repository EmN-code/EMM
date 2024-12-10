#include <zephyr/kernel.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buf[128];
    int r;

    printk("Starting CoAP client\n");

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Set server address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &addr.sin_addr);

    // Initialize CoAP packet
    r = coap_packet_init(&request, request_buf, sizeof(request_buf), 1, COAP_TYPE_CON, 8, coap_next_token(), COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        printk("Failed to initialize CoAP request\n");
        close(sock);
        return;
    }

    // Set URI path
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        printk("Failed to add URI path to request\n");
        close(sock);
        return;
    }

    // Set payload
    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to add payload marker to request\n");
        close(sock);
        return;
    }
    r = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    if (r < 0) {
        printk("Failed to add payload to request\n");
        close(sock);
        return;
    }

    // Send CoAP request
    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return;
    }

    printk("CoAP request sent successfully\n");

    // Close socket
    close(sock);
}