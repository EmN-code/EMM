#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include <string.h>

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

const char *EMQX_SERVER_IP = "47.102.103.1";
const char *EMQX_URI_PATH = "/coap/test";

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        printf("error in resp_handler\n");
        return;
    }
    printf("response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .port = CONFIG_GCOAP_PORT };  
    coap_pkt_t pdu;

    // Convert IP address to appropriate format
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Send a CoAP request
    ssize_t bytes_sent = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes_sent <= 0) {
        printf("Error sending request\n");
        return -1;
    }
    printf("Request sent successfully\n");

    return 0;
}