#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <net/coap_link_format.h>
#include <net/net_pkt.h>
#include <net/net_ip.h>
#include <net/net_context.h>

#define COAP_MAX_MSG_LEN 256

#define COAP_SERVER_IP_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

void main(void) {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_buf[COAP_MAX_MSG_LEN];
    int sock;
    int r;

    /* Create UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP_ADDR, &server_addr.sin_addr);

    /* Initialize CoAP packet */
    r = coap_packet_init(&request, request_buf, sizeof(request_buf),
                         COAP_VERSION_1, COAP_TYPE_CON, 8,
                         coap_next_token(), COAP_METHOD_POST,
                         coap_next_id());
    if (r < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return;
    }

    /* Set URI path for the request */
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                  COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        printk("Failed to set URI path in CoAP request\n");
        close(sock);
        return;
    }

    /* Set payload for the request */
    const char payload[] = "Hello, COAP Cloud";
    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to set payload marker in CoAP request\n");
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)payload, sizeof(payload) - 1);
    if (r < 0) {
        printk("Failed to set payload in CoAP request\n");
        close(sock);
        return;
    }

    /* Send CoAP request */
    r = sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return;
    }

    printk("CoAP request sent successfully\n");

    close(sock);
}