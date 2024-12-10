#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "time_units.h"
#include "arpa/inet.h"

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 256

static const char* EMQX_URI_PATH = "/coap/test";
static const char* EMQX_SERVER_IP = "47.102.103.1";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("CoAP request timeout\n");
        return;
    }

    if (pdu->payload_len != 0) {
        printf("Response from server: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
    else {
        printf("No response from server\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    size_t hdrlen = coap_hdr_len(&pdu);

    result = coap_pkt_append_payload_marker(&pdu);
    if (result == 0) {
        printf("Error adding payload marker\n");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    result = coap_pkt_append_payload(&pdu, (uint8_t *)payload, payload_len);
    if (result == 0) {
        printf("Error adding payload\n");
        return 1;
    }

    result = gcoap_req_send(buf, hdrlen + payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}