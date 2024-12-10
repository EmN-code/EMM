#include <net/net_core.h>
#include <net/net_if.h>
#include <net/coap.h>
#include <net/coap_utils.h>
#include <kernel.h>
#include <zephyr.h>

#define COAP_MAX_MSG_LEN 256

static const char *server_ip = "47.102.103.1";
static const uint16_t server_port = 5683;
static const char *uri_path = "/coap/test";

static uint8_t coap_buf[COAP_MAX_MSG_LEN];

void main(void) {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    int sock, r;

    /* Initialize network */
    do {
        k_msleep(1000);
        struct net_if *iface = net_if_get_default();
    } while (!net_if_is_up(net_if_get_default()));

    /* Create a socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return;
    }

    /* Setup CoAP server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    /* Initialize CoAP packet */
    r = coap_packet_init(&request, coap_buf, sizeof(coap_buf), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        printk("Failed to initialize request: %d\n", r);
        close(sock);
        return;
    }

    /* Set URI path */
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));

    /* Set payload */
    const char *payload = "Hello, COAP Cloud";
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    /* Send the CoAP request */
    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send request: %d\n", errno);
    }

    /* Close the socket */
    close(sock);
    printk("CoAP POST request sent\n");
}