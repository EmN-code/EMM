#include <stdio.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;

static void send_coap_post(void)
{
    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t bytes;

    /* Initialize the remote endpoint */
    ipv4_addr_t addr;
    ipv4_addr_from_str(&addr, SERVER_ADDR);
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = atoi(SERVER_PORT);
    memcpy(&remote.addr.ipv4, &addr, sizeof(addr));

    /* Initialize the CoAP PDU */
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_TYPE_CON, NULL, 0);
    coap_hdr_set_code(&pdu, COAP_METHOD_POST);
    coap_opt_add_string(&pdu, COAP_OPT_URI_PATH, URI_PATH, '/');
    coap_payload_put_bytes(&pdu, PAYLOAD, strlen(PAYLOAD));

    /* Send the CoAP request */
    bytes = sock_udp_send(NULL, buf, coap_get_total_hdr_len(&pdu) + pdu.payload_len, &remote);
    if (bytes < 0) {
        printf("Error sending CoAP request: %d\n", bytes);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void)
{
    printf("RIOT CoAP client example\n");

    /* Send the CoAP POST request */
    send_coap_post();

    return 0;
}