#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>  // For inet_pton
#include <net/gcoap.h>  // CoAP library in RIOT
#include <net/gnrc/netif.h>  // For network interface
#include "shell.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;
    sock_udp_ep_t remote;

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("ERROR: Unable to parse IP");
        return 1;
    }
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    // Initialize CoAP request
    if ((res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH)) < 0) {
        printf("gcoap_req_init failed with %d\n", res);
        return 1;
    }

    // Format CoAP payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP message sent");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response code: %u\n", coap_get_code(pdu));  // Correct function usage
    } else {
        puts("No response received");
    }
}