#include <zephyr/types.h>
#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST

void send_coap_request(void)
{
    int sock;
    struct sockaddr_in6 addr;
    struct coap_packet request;
    uint8_t request_buffer[256];
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET;
    addr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin6_addr);

    coap_packet_init(&request, request_buffer, sizeof(request_buffer), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_PATH, strlen(COAP_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    ret = sendto(sock, request_buffer, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent\n");
    }

    close(sock);
}

void main(void)
{
    printk("Starting CoAP client\n");
    send_coap_request();
}