#include <zephyr.h>
#include <stdio.h>
#include <net/socket.h>
#include <net/coap.h>
#include <net/coap_sock.h>

#define COAP_MAX_MSG_LEN 256
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define POST_PAYLOAD "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    uint8_t request_buffer[COAP_MAX_MSG_LEN];
    ssize_t result;

    printf("Starting CoAP client\n");

    // Create a CoAP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket, error %d\n", errno);
        return;
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr));

    // Prepare CoAP request
    struct coap_packet request;
    coap_packet_init(&request, request_buffer, sizeof(request_buffer),
                     COAP_VERSION_1,
                     COAP_TYPE_CON,
                     COAP_TOKEN_MAX_LEN,
                     coap_next_token(),
                     COAP_METHOD_POST,
                     coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, POST_PAYLOAD, strlen(POST_PAYLOAD));

    // Send CoAP request
    result = sendto(sock, request_buffer, request.offset, 0,
                    (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result < 0) {
        printf("Failed to send CoAP request, error %d\n", errno);
    } else {
        printf("CoAP POST request sent successfully!\n");
    }

    // Close the socket
    close(sock);
}