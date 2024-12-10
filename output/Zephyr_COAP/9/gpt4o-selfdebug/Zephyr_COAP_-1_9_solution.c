#include <zephyr.h>
#include <sys/socket.h>
#include <net/coap.h>
#include <net/socket.h>

#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     5683
#define COAP_URI_PATH   "/coap/test"

void main(void) {
    struct coap_packet request;
    struct sockaddr_in6 addr;
    int sock;
    uint8_t coap_buf[256];
    const char *hello_message = "Hello, COAP Cloud";
    int ret;

    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket.\n");
        return;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDR, &addr.sin6_addr);

    ret = coap_packet_init(&request, coap_buf, sizeof(coap_buf), COAP_VERSION_1, COAP_TYPE_CON, 0,
                           NULL, 0, coap_next_id());
    if (ret < 0) {
        printf("Failed to init CoAP message.\n");
        return;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH,
                                    strlen(COAP_URI_PATH));
    if (ret < 0) {
        printf("Failed to append option.\n");
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printf("Failed to append payload marker.\n");
        return;
    }

    ret = coap_packet_append_payload(&request, (uint8_t *)hello_message, strlen(hello_message));
    if (ret < 0) {
        printf("Failed to append payload.\n");
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        printf("Failed to send CoAP request.\n");
        return;
    }

    printf("CoAP message sent successfully.\n");

    close(sock);
}