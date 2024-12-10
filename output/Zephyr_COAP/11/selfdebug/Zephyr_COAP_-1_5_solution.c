
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void send_coap_request(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP packet
    if (coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 4, NULL, COAP_METHOD_POST, coap_next_id()) < 0) {
        printf("Failed to initialize CoAP packet\n");
        close(sock);
        return;
    }

    // Add URI path option
    if (coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)URI_PATH, strlen(URI_PATH)) < 0) {
        printf("Failed to add URI path option\n");
        close(sock);
        return;
    }

    // Add payload
    if (coap_packet_append_payload_marker(&request) < 0 ||
        coap_packet_append_payload(&request, (uint8_t *)PAYLOAD, strlen(PAYLOAD)) < 0) {
        printf("Failed to add payload\n");
        close(sock);
        return;
    }

    // Send the CoAP request
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    close(sock);
}

int main(void) {
    printf("Starting CoAP client\n");
    send_coap_request();
    return 0;
}
