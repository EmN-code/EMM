#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define POST_DATA "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

void main(void) {
    struct sockaddr_in addr;
    int sock;
    struct coap_packet request;
    uint8_t coap_buf[256];
    
    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Set server address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    // Initialize CoAP request
    coap_packet_init(&request, coap_buf, sizeof(coap_buf), 1, COAP_METHOD_POST, 0, NULL, 0, NULL, 0);
    coap_packet_append_uri_path_from_string(&request, URI_PATH);
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)POST_DATA, strlen(POST_DATA));

    // Send CoAP request
    if (sendto(sock, coap_buf, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
}