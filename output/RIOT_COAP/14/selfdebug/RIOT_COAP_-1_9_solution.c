#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    char* data = "Hello, COAP Cloud";
    ssize_t len;
    sock_udp_ep_t remote = { . family = AF_INET, .port = COAP_SERVER_PORT };
    
    /* Convert IP address from text to binary form */
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to convert IP address\n");
        return 1;
    }

    /* Initialize the CoAP packet buffer */
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    /* Set content-format option to indicate plain text */
    coap_opts_extend(&pdu, COAP_OPTION_CONTENT_FORMAT, COAP_CT_TEXT_PLAIN);

    /* Set payload */
    memcpy(coap_payload(&pdu), data, strlen(data));
    pdu.payload_len = strlen(data);

    /* Send CoAP request */
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send request\n");
        return 1;
    }

    return 0;
}