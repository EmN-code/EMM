#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <stdio.h>
#include <string.h>

int main() {
    struct sockaddr_in addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buffer[128];
    const char *server_ip = "47.102.103.1";
    const uint16_t server_port = 5683;
    const char *uri_path = "/coap/test";
    const char *payload = "Hello, COAP Cloud";

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    // Configure destination address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &addr.sin_addr);

    // Prepare CoAP request
    coap_packet_init(&request, request_buffer, sizeof(request_buffer), 1, COAP_TYPE_CON, COAP_TOKEN_NONE, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    // Send CoAP request
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printf("CoAP request sent successfully\n");
    close(sock);
    return 0;
}