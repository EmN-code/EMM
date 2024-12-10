#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main() {
    int sock;
    struct sockaddr_in server_addr;
    uint8_t buffer[256];
    struct coap_packet request;
    int r;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket.\n");
        return -1;
    }

    // Configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP packet
    r = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 8, 
                         NULL, COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        printf("Failed to initialize CoAP packet.\n");
        close(sock);
        return -1;
    }

    // Append URI options
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));

    // Append payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)MESSAGE, strlen(MESSAGE));

    // Send the CoAP request
    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printf("Failed to send CoAP request.\n");
        close(sock);
        return -1;
    }

    printf("CoAP request sent successfully.\n");
    close(sock);
    return 0;
}