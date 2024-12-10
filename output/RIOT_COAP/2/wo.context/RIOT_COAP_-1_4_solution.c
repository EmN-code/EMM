#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>  // Corrected this include to fix 'inet_pton' error
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP       "47.102.103.1"
#define COAP_SERVER_PORT     CONFIG_GCOAP_PORT
#define COAP_URI_PATH        "/coap/test"
#define COAP_PAYLOAD         "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_ERROR) {
        puts("CoAP request failed");
    }
    else {
        printf("CoAP response received: %u\n", pdu->payload_len);
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize buffer and remote endpoint
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    // Add payload
    if (coap_opt_add_uint(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT) < 0) {
        puts("Error adding options");
        return -1;
    }
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Correctly assigning IPv4 address
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address");
        return -1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    // Send the request
    if (gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
        puts("CoAP POST request sent successfully!");
    } else {
        puts("Error sending CoAP POST request");
    }

    return 0;
}