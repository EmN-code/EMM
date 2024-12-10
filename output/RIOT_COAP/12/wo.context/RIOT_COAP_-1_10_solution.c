#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <gcoap_example_cmd.h>

#define SERVER_IP      "47.102.103.1"
#define COAP_PORT      (5683)
#define URI_PATH       "/coap/test"
#define PAYLOAD        "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    if (pdu != NULL) {
        printf("Response received: %u\n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    ssize_t bytes;

    /* convert IP address */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    gcoap_payload(pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    bytes = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        printf("Error sending request\n");
    }
    return 0;
}