#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

void main(void) {
    struct sockaddr_in server_addr;
    struct zsock_addrinfo hints, *res;
    struct coap_packet request;
    uint8_t coap_token[8] = { 0x52, 0x45, 0x51, 0x30, 0x42, 0x4D, 0x4E, 0x32 };
    const char *payload = "Hello, COAP Cloud";
    uint8_t *data;
    int r;

    printk("Starting CoAP Client\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    r = getaddrinfo(SERVER_ADDR, NULL, &hints, &res);
    if (r != 0) {
        printk("ERROR: getaddrinfo failed %d\n", r);
        return;
    }
    
    r = coap_packet_init(&request, data, sizeof(data), COAP_VERSION_1, COAP_TYPE_CON, sizeof(coap_token), coap_token, COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        printk("ERROR: unable to init CoAP packet %d\n", r);
        return;
    }

    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (r < 0) {
        printk("ERROR: unable to append URI_PATH option %d\n", r);
        return;
    }

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("ERROR: unable to append payload marker %d\n", r);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));
    if (r < 0) {
        printk("ERROR: unable to append payload %d\n", r);
        return;
    }

    r = zsock_sendto(res->ai_socktype, request.data, request.offset, 0, res->ai_addr, res->ai_addrlen);
    if (r < 0) {
        printk("ERROR: zsock_sendto failed %d\n", r);
        return;
    }

    printk("CoAP POST request sent: %s\n", payload);
    freeaddrinfo(res);
}
