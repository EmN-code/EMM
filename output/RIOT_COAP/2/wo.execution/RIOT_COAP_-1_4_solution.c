#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout");
        return;
    }
    unsigned long len = coap_get_payload_len(pdu);
    printf("Response %u: %s\n", (unsigned)pdu->hdr->code, len ? (char *)coap_get_payload(pdu) : "");
}

int main(void) {
    // Prepare the remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Buffer for CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("gcoap_req_init() failed");
        return 1;
    }

    // Add payload to CoAP packet
    const char *payload = "Hello, COAP Cloud";
    len += coap_pkt_append_payload(&pdu, (const uint8_t *)payload, strlen(payload));

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("gcoap_req_send() failed");
        return 1;
    }

    puts("Request sent");
    return 0;
}