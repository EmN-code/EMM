#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "arpa/inet.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_METHOD_POST 2
#define URI_PATH "/coap/test"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static sock_udp_ep_t remote;

static void _resp_handler(coap_pkt_t* pdu, const sock_udp_ep_t* remote, void* ctx) {
    (void)ctx;
    char* content = (char*)coap_get_payload(pdu);
    printf("Response: %.*s\n", pdu->payload_len, content);
}

int main(void) {
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }
    remote.port = SERVER_PORT;

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = coap_hdr_set_payload(pdu.hdr, (uint8_t*)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    ssize_t result = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}