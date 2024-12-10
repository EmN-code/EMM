#include <stdio.h>
#include <string.h>
#include <net/gnrc/netif.h>
#include <net/gcoap.h>
#include <net/coap.h>
#include <msg.h>

#define COAP_URI_PATH "/coap/test"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683

ssize_t send_coap_post(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    /* Initialize the CoAP PDU */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    pdu.payload_len = 17;

    /* Prepare remote server endpoint */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    /* Send the request using gcoap */
    res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote,
                         _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return res;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu != NULL) {
        printf("Received response: %u\n", coap_get_code(pdu));
    }
    else {
        printf("Request timeout or error\n");
    }
}

int main(void) {
    printf("Starting CoAP POST client...\n");
    while(send_coap_post() <= 0) {
        printf("Failed to send CoAP request, retrying...\n");
    }
    return 0;
}
