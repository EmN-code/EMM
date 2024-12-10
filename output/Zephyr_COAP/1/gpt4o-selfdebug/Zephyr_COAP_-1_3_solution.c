#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <net/net_ip.h>
#include <net/net_core.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in6 addr;
    struct coap_packet request;
    uint8_t request_buf[128];
    int sock;
    int r;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Error: Unable to create socket\n");
        return;
    }

    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDR, &addr.sin6_addr);

    r = coap_packet_init(&request, request_buf, sizeof(request_buf), 1, COAP_TYPE_CON, 8, coap_next_token(), COAP_METHOD_POST, coap_next_mid());
    if (r < 0) {
        printk("Error: Unable to initialize CoAP request\n");
        close(sock);
        return;
    }

    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        printk("Error: Unable to append option\n");
        close(sock);
        return;
    }

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Error: Unable to append payload marker\n");
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    if (r < 0) {
        printk("Error: Unable to append payload\n");
        close(sock);
        return;
    }

    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printk("Error: Unable to send CoAP request\n");
        close(sock);
        return;
    }

    printk("CoAP request sent successfully\n");
    close(sock);
}