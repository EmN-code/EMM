#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/nanocoap.h>  // Include CoAP library for nanocoap
#include <net/netif.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT // Using default CoAP port 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote; // this is not used
    if (memo->state == GCOAP_MEMO_COMPLETE) {
        if (pdu->code == COAP_CODE_CREATED || pdu->code == COAP_CODE_CHANGED) {
            printf("Success: code %u
", pdu->code);
        } else {
            printf("Error: code %u
", pdu->code);
        }
    } else {
        printf("No response yet
");
    }
}

static int send_coap_request(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* initialize CoAP pdu */
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    /* set the payload */
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    /* configure remote CoAP server */
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    /* send CoAP request */
    return gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void)
{
    printf("Initiating CoAP request...
");
    int res = send_coap_request();
    if (res >= 0) {
        printf("CoAP request sent successfully!
");
    } else {
        printf("Failed to send CoAP request: %d
", res);
    }
    return 0;
}