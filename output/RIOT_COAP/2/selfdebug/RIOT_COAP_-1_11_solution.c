#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    gcoap_req_send(buf, pdu.payload_len + (pdu.payload - buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}