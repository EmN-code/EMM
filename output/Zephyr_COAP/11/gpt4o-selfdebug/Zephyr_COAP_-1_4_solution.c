#include <zephyr/types.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define POST_MSG "Hello, COAP Cloud"

#define COAP_URI_PATH "/coap/test"
#define COAP_VERSION 1
#define COAP_TYPE COAP_TYPE_CON
#define COAP_METHOD COAP_METHOD_POST

#define STACK_SIZE 1024
#define THREAD_PRIORITY K_PRIO_COOP(8)

void main(void) {
    static char buf[128];
    struct coap_packet request;
    int r;

    struct sockaddr_in addr;
    int sock;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Cannot create socket\n");
        return;
    }

    r = coap_packet_init(&request, buf, sizeof(buf), COAP_VERSION, COAP_TYPE, COAP_TOKEN_NONE, 0, COAP_METHOD, COAP_URI_PATH);
    if (r < 0) {
        printk("Failed to init CoAP packet: %d\n", r);
        close(sock);
        return;
    }

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to append payload marker: %d\n", r);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)POST_MSG, strlen(POST_MSG));
    if (r < 0) {
        printk("Failed to append payload: %d\n", r);
        return;
    }

    sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));

    close(sock);
}