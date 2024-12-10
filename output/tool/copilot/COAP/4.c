#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static void send_coap_request(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    uint8_t token[2] = {0x12, 0x34}; // Example token

    // Set server address
    ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP);

    // Initialize CoAP packet
    coap_pkt_init(&pdu, buf, sizeof(buf), 1, COAP_METHOD_POST, token, sizeof(token));
    coap_hdr_set_type(&pdu, COAP_TYPE_CON);
    coap_opt_add_string(&pdu, COAP_OPT_URI_PATH, URI_PATH, '/');
    coap_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    if (sock_udp_send(NULL, buf, coap_get_total_hdr_len(&pdu) + pdu.payload_len, &server) < 0) {
        puts("Error sending CoAP request");
    } else {
        puts("CoAP request sent");
    }
}

int main(void) {
    // Initialize networking
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(0);
    if (!netif) {
        puts("Error: No network interface found");
        return 1;
    }

    // Send CoAP request
    send_coap_request();

    // Wait for response (optional)
    xtimer_sleep(5);

    return 0;
}