#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>

void main(void) {
    struct sockaddr_in addr;
    int sock;
    struct coap_packet request;
    uint8_t *data = "Hello, COAP Cloud";
    uint8_t token[8];
    
    printk("CoAP client sample starting\n");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &addr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    coap_packet_init(&request, COAP_TYPE_CON, COAP_METHOD_POST, 0, token, sizeof(token));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, data, strlen(data));
    coap_packet_append_uri_path(&request, "/coap/test");

    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
}