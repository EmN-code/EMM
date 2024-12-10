#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;   // Not used in this basic handler
    if (pdu != NULL) {
        printf("Response received: code=%d\n", coap_get_code(pdu));
    }
    else {
        printf("-- No response received\n");
    }
}

void send_coap_request(void) {
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return;
    }
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    size_t len;
    coap_pkt_t pdu;
    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE,
                             COAP_METHOD_POST, EMQX_URI_PATH);
    if (res < 0) { 
        printf("Error: unable to initialize request, code %d\n", res);
        return;
    }
    len = res;
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL,
                       GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send request\n");
    }
}

int main(void) {
    printf("Sending CoAP request\n");
    send_coap_request();
    return 0;
}
