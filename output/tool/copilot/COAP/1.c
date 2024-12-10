#include <stdio.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

void send_coap_post(void)
{
    uint8_t buf[128];
    coap_pkt_t pkt;
    uint8_t *payload;
    size_t payload_len = strlen(PAYLOAD);

    // Set the server address
    ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_ADDR);

    // Initialize the CoAP packet
    coap_pkt_init(&pkt, buf, sizeof(buf), 0);
    coap_hdr_set_type(&pkt, COAP_TYPE_CON);
    coap_hdr_set_code(&pkt, COAP_METHOD_POST);
    coap_hdr_set_id(&pkt, coap_get_id());

    // Set the URI path
    coap_opt_add_string(&pkt, COAP_OPT_URI_PATH, URI_PATH, '/');

    // Set the payload
    payload = coap_payload(&pkt);
    memcpy(payload, PAYLOAD, payload_len);
    coap_set_payload_len(&pkt, payload_len);

    // Send the CoAP request
    ssize_t res = sock_udp_send(NULL, buf, coap_get_total_len(&pkt), &server);
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void)
{
    // Initialize the network interface
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(0);
    if (netif == NULL) {
        printf("Error: No network interface found\n");
        return 1;
    }

    // Send the CoAP POST request
    send_coap_post();

    return 0;
}