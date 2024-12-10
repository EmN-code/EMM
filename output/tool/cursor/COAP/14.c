#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/coap.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[128];
    ssize_t res;

    // Resolve the server address
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    // Initialize CoAP packet
    coap_pkt_t pdu;
    coap_hdr_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_TYPE_CON, 0);
    coap_opt_add_uri_path(&pdu, URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send the CoAP request
    res = sock_udp_send(NULL, buf, coap_hdr_len(&pdu) + pdu.payload_len, &remote);
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
        return 1;
    }

    printf("CoAP POST request sent to %s:%d\n", SERVER_ADDR, SERVER_PORT);

    return 0;
}