#include <net/sock/udp.h>
#include <net/sock.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>

#define COAP_SERVER "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128 // Removed redefinition in the code if it exists elsewhere

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Received response\n");
}

int main(void) {
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = COAP_PORT
    };
    
    if (inet_pton(AF_INET, COAP_SERVER, &remote.addr.ipv4) == 0) {
        puts("Error: unable to parse server IP");
        return -1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}