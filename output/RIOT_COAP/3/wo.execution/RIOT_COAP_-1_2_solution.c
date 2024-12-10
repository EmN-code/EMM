#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "esp_wifi.h"
#include "arpa/inet.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define DATA "Hello, COAP Cloud"
#define GCOAP_PDU_BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout");
        return;
    }
    
    printf("Response received\n");
    if (memo->state == GCOAP_MEMO_ERR) {
        puts("gcoap: error in response");
        return;
    }

    printf("payload: %.*s\n", pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;

    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, DATA, sizeof(DATA));
    pdu.payload_len = sizeof(DATA);

    result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (result <= 0) {
        printf("gcoap_req_send() failed with %d\n", (int) result);
    }

    return 0;
}