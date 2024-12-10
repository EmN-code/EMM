#include <stdio.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT "5683"
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void send_coap_post(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = atoi(COAP_SERVER_PORT) };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_ADDR);

    uint8_t buf[128];
    coap_pkt_t pdu;
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_TYPE_CON, COAP_METHOD_POST, coap_get_id());
    coap_opt_add_uri_path(&pdu, COAP_URI_PATH);
    coap_payload(&pdu, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    ssize_t res = nanocoap_request(&pdu, &remote, buf, sizeof(buf));
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", (int)res);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void)
{
    printf("Starting CoAP client example\n");

    // Initialize the network interface
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(0);
    if (netif == NULL) {
        printf("Error: No network interface found\n");
        return 1;
    }

    // Send CoAP POST request
    send_coap_post();

    return 0;
}