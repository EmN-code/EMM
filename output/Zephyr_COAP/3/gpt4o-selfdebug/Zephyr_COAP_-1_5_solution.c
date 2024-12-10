#include <zephyr/zephyr.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <stdio.h>

#define COAP_HOSTNAME "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"

#define COAP_BUF_SIZE 256
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(COAP_PORT)
    };
    inet_pton(AF_INET, COAP_HOSTNAME, &addr.sin_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    uint8_t coap_buf[COAP_BUF_SIZE];
    struct coap_packet request;

    int r = coap_packet_init(&request, coap_buf, sizeof(coap_buf), COAP_VERSION_1,
                             COAP_TYPE_CON, 0, coap_next_token(), COAP_METHOD_POST,
                             coap_next_id());
    if (r < 0) {
        printf("Failed to init CoAP packet\n");
        return;
    }

    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                  COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        printf("Failed to append CoAP option\n");
        return;
    }

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printf("Failed to append CoAP payload marker\n");
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    if (r < 0) {
        printf("Failed to append CoAP payload\n");
        return;
    }

    r = sendto(sock, coap_buf, coap_packet_get_len(&request), 0,
               (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printf("Failed to send CoAP request\n");
        return;
    }

    close(sock);
    printf("CoAP POST request sent successfully\n");
}