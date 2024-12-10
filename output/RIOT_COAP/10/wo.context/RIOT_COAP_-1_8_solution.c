#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

const char EMQX_SERVER_IP[] = "47.102.103.1";
const char EMQX_URI_PATH[] = "/coap/test";

// Buffer for sending data
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling code here
    (void)memo;
    (void)pdu;
    (void)remote;
}

int main(void) {
    // Define endpoint
    sock_udp_ep_t remote;
    int result;

    // Set the server address
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    // Set the server port
    remote.family = AF_INET;
    remote.port = 5683;

    // Prepare CoAP packet
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    // Add payload
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res < 0) {
        printf("Request send failed\n");
        return -1;
    }

    printf("Request sent successfully\n");
    return 0;
}