#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define BUFFER_SIZE 128

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    uint8_t buffer[BUFFER_SIZE];
    struct coap_packet request;

    printf("Starting CoAP POST request example\n");

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize CoAP packet
    coap_packet_init(&request, buffer, BUFFER_SIZE, 1, COAP_TYPE_CON, COAP_CODE_POST, coap_next_token());
    coap_packet_append_uri_path_from_string(&request, COAP_URI_PATH);
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send CoAP request
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Close socket
    close(sock);
}