#include <stdio.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = COAP_SERVER_PORT
};

static void send_coap_post(void)
{
    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t bytes;

    /* Set the remote address */
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_ADDR);

    /* Initialize the CoAP PDU */
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_TYPE_CON, COAP_METHOD_POST, coap_get_id());
    coap_hdr_set_uri_path(&pdu, COAP_URI_PATH);
    coap_payload(&pdu, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

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
    printf("RIOT CoAP POST example\n");

    /* Send the CoAP POST request */
    send_coap_post();

    /* Sleep to allow the request to be sent */
    xtimer_sleep(5);

    return 0;
}