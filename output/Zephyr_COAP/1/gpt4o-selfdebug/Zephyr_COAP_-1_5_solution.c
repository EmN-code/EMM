#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in6 addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buf[256];
    int r;

    printk("Sending CoAP POST request to %s:%d\n", COAP_SERVER_IP, COAP_SERVER_PORT);

    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET6, COAP_SERVER_IP, &addr.sin6_addr);

    sock = socket(addr.sin6_family, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    r = coap_packet_init(&request, request_buf, sizeof(request_buf),
                         1, COAP_TYPE_CON, 8, coap_next_token(),
                         COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        printk("Failed to init CoAP packet\n");
        close(sock);
        return;
    }

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    r = sendto(sock, request_buf, coap_packet_get_total_size(&request), 0,
               (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
}