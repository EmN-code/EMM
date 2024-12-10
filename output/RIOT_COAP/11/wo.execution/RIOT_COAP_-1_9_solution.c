#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <stdio.h>

#define COAP_PORT    (5683)
#define SERVER_ADDR  "47.102.103.1"
#define URI_PATH     "/coap/test"
#define PAYLOAD      "Hello, COAP Cloud"

static void _resp_handler(const coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    printf("Response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = COAP_PORT;
    
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        puts("inet_pton failed");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("gcoap_req_send failed");
        return 1;
    }

    puts("CoAP request sent");

    return 0;
}