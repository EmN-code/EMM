#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define SERVER_IP_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    
    if (inet_pton(AF_INET, SERVER_IP_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error parsing IP address\n");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);
    
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("Request sent\n");
    return 0;
}