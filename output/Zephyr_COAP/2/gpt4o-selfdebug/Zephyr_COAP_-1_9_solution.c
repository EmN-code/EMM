#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_buf[256];
    int sock;

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    zsock_inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // Create socket
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Initialize CoAP packet
    coap_packet_init(&request, request_buf, sizeof(request_buf), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)MESSAGE, strlen(MESSAGE));

    // Send CoAP request
    if (zsock_sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    // Close socket
    zsock_close(sock);
}