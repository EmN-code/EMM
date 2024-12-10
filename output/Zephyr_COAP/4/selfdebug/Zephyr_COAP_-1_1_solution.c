#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_METHOD COAP_METHOD_POST
#define COAP_TYPE COAP_TYPE_CON
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main() {
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buffer[128];
    int result;

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return -1;
    }

    // Initialize CoAP packet
    coap_packet_init(&request, request_buffer, sizeof(request_buffer), COAP_VERSION_1, COAP_TYPE, 0, 0, NULL, COAP_METHOD, NULL);

    // Set URI path
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)URI_PATH, strlen(URI_PATH));

    // Set payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    result = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result < 0) {
        fprintf(stderr, "Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printf("CoAP POST request sent successfully\n");

    // Close socket
    close(sock);
    return 0;
}