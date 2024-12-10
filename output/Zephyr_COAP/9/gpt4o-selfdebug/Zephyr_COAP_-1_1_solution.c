#include <zephyr/net/tls_credentials.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_MSG_CONTENT "Hello, COAP Cloud"
#define COAP_PATH "/coap/test"

void main(void)
{
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // Initialize CoAP packet for a POST request
    coap_packet_init(&request, buffer, sizeof(buffer),
                     COAP_VERSION_1, COAP_TYPE_CON,
                     0, NULL, COAP_METHOD_POST, coap_next_token());

    // Set URI Path
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_PATH,
                              strlen(COAP_PATH));

    // Append the payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, COAP_MSG_CONTENT, strlen(COAP_MSG_CONTENT));

    // Send the CoAP request
    ret = sendto(sock, request.data, request.offset, 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP POST request sent successfully\n");
    }

    // Close socket
    close(sock);
}