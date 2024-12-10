// Including necessary headers in descending order by their first letter
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define SEND_DATA "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

void main(void) {
    // Socket address setup
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    // Create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Set up CoAP packet
    struct coap_packet request;
    uint8_t request_buf[128];

    coap_packet_init(&request, request_buf, sizeof(request_buf),
                     1, COAP_TYPE_CON, COAP_TOKEN_NONE,
                     coap_next_id(), COAP_METHOD_POST, 
                     &URI_PATH[1]); // Strip leading '/' for path

    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)SEND_DATA, strlen(SEND_DATA));

    // Send request
    if (sendto(sock, request.data, request.offset,
               0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    // Close socket
    close(sock);
}