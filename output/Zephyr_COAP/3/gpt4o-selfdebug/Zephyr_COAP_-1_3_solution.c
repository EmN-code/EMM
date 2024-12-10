# Include necessary headers for networking and CoAP functionality
#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>
#include <stdio.h>

#define STACK_SIZE 2048
#define PRIORITY 7
#define COAP_PORT 5683
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void coap_send_request(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buf[256];
    int ret;

    // Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    // Prepare CoAP packet
    ret = coap_packet_init(&request, buf, sizeof(buf), 1, COAP_TYPE_CON,
                           8, coap_next_id(), COAP_METHOD_POST,
                           coap_encode_varint(obs + 1));
    if (ret < 0) {
        printf("Failed to initialize CoAP packet\n");
        close(sock);
        return;
    }

    // Set URI path
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                              COAP_URI_PATH, strlen(COAP_URI_PATH));

    // Set payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD,
                               strlen(COAP_PAYLOAD));

    // Send CoAP request
    ret = sendto(sock, request.data, request.offset, 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Close socket
    close(sock);
}

void main(void) {
    printf("Starting CoAP client example\n");
    coap_send_request();
}