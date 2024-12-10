#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/net_ip.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define SERVER_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buffer[128];
    struct coap_option options[1];

    /* Initialize the server address structure */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    /* Create a UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    /* Initialize CoAP packet */
    coap_packet_init(&request, request_buffer, sizeof(request_buffer), COAP_VERSION_1);
    coap_packet_append_request(&request, COAP_TYPE_CON, COAP_METHOD_POST, coap_next_id());

    /* Append CoAP URI path option */
    options[0].delta = COAP_OPTION_URI_PATH;
    options[0].len = sizeof(SERVER_PATH) - 1;
    options[0].value = (const uint8_t *)SERVER_PATH;
    coap_packet_append_options(&request, options, ARRAY_SIZE(options));

    /* Append payload */
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (const uint8_t *)PAYLOAD, sizeof(PAYLOAD) - 1);

    /* Send CoAP request to server */
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP POST request sent successfully\n");
    }

    /* Close the socket */
    close(sock);

    return 0;
}