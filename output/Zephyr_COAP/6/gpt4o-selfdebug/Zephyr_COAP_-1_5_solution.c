#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define COAP_MAX_MSG_LEN 256
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683

void main(void) {
    static uint8_t payload[] = "Hello, COAP Cloud";
    uint8_t coap_msg[COAP_MAX_MSG_LEN];
    struct coap_packet request;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &addr.sin_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    coap_packet_init(&request, coap_msg, sizeof(coap_msg),
                     1, COAP_TYPE_CON, 8, coap_next_token(),
                     COAP_METHOD_POST, coap_next_id());

    coap_packet_append_uri_path(&request, "coap/test");
    coap_packet_append_payload_marker(&request);

    coap_packet_append_payload(&request, payload, sizeof(payload) - 1);

    if (sendto(sock, coap_msg, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printk("CoAP message sending failed\n");
    } else {
        printk("CoAP message sent successfully\n");
    }

    close(sock);
}