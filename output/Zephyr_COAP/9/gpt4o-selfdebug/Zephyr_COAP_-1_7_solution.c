#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <stdio.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

#define COAP_TOKEN_MAX_LEN 8
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void)
{
    struct sockaddr_in server_addr;
    struct coap_packet request;
    struct coap_option options[4];
    uint8_t coap_buf[128];
    uint8_t token[COAP_TOKEN_MAX_LEN];
    int sock;

    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    // Initialize CoAP packet
    coap_packet_init(&request, coap_buf, sizeof(coap_buf), 1, COAP_TYPE_CON, 
                     COAP_TOKEN_MAX_LEN, token, COAP_METHOD_POST, coap_next_id());

    // Set URI path
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, 
                              COAP_URI_PATH, strlen(COAP_URI_PATH));

    // Add payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, 
                               sizeof(COAP_PAYLOAD) - 1);

    // Send request
    if (sendto(sock, request.data, request.offset, 0, 
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Close socket
    close(sock);
}