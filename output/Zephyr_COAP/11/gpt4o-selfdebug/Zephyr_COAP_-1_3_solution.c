// Required header files
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <stdio.h>

// Main function
int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[128];
    struct coap_packet request;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

    // Initialize the CoAP request
    coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1);
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, "coap/test", strlen("coap/test"));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send CoAP POST request
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printf("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}