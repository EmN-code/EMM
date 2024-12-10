#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <gcoap.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP)
        printf("CoAP Response received\n");
    else
        printf("No CoAP response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    /* Configure remote address */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse remote address");
        return -1;
    }
  
    /* Prepare CoAP message */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);  /* Corrected usage */
    len += coap_payload(pdu.hdr, "Hello, COAP Cloud", strlen(PAYLOAD));  /* Set Payload */
  
    /* Send CoAP request */
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == -1) {
        puts("Error: unable to send CoAP request");
        return -1;
    }
  
    puts("CoAP request sent");
    return 0;
}