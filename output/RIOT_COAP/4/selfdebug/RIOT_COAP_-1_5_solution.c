#include <string.h>
#include "gcoap.h"
#include "net/sock/udp.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    ssize_t res;
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");
    
    res = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request: %d\n", res);
    }

    return 0;
}