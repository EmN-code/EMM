#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define GCOAP_MEMO_TIMEOUT 30

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (!memo || !pdu) {
        printf("Error: no memo or pdu received\n");
        return;
    }

    switch (memo->state) {
        case GCOAP_MEMO_RESP:
            printf("Response received\n");
            printf("Payload: %.*s\n", pdu->payload_len, (char *)pdu->payload);
            break;

        case GCOAP_MEMO_TIMEOUT:
            printf("Error: request timed out\n");
            break;

        default:
            printf("Error: unexpected memo state\n");
            break;
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (len == 0) {
        printf("Error: unable to initialize request\n");
        return 1;
    }

    char payload[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to encode server address\n");
        return 1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    int res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: unable to send request\n");
        return 1;
    }

    printf("Request sent successfully\n");
    return 0;
}
