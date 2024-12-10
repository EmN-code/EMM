#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <net/coap_link_format.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI "/coap/test"

#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in6 addr;
    struct coap_packet request;
    uint8_t *data, token[8] = {0};
    int ret, sock;

    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDR, &addr.sin6_addr);

    ret = coap_packet_init(&request, data, sizeof(data), COAP_VERSION_1, COAP_TYPE_CON, 8, token, COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to init CoAP packet: %d\n", ret);
        close(sock);
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker: %d\n", ret);
        close(sock);
        return;
    }

    ret = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, sizeof(COAP_PAYLOAD) - 1);
    if (ret < 0) {
        printk("Failed to append payload: %d\n", ret);
        close(sock);
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        printk("Failed to send CoAP request: %d\n", errno);
        close(sock);
        return;
    }

    printk("CoAP POST sent\n");
    close(sock);
}