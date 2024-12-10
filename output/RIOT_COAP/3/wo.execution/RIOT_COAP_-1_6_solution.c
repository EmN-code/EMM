#include "thread.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "esp_wifi.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "arpa/inet.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "dht_params.h"

#define SERVER_ADDR       "47.102.103.1"
#define SERVER_PORT       CONFIG_GCOAP_PORT
#define COAP_PATH          "/coap/test"
#define COAP_MSG          "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout for CoAP request");
        return;
    }
    if (pdu->hdr->code == COAP_CODE_231) {
        puts("Success!");
    }
    else {
        printf("Error: code %u
", pdu->hdr->code);
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[128];
    coap_pkt_t pdu;
    ip_addr_t addr;

    if (inet_pton(AF_INET, SERVER_ADDR, &addr) != 1) {
        puts("Error: unable to parse destination address");
        return 1;
    }
    memcpy(&remote.addr, &addr, sizeof(addr));
    
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_PATH);
    if (len == 0) {
        puts("Error creating CoAP request");
        return 1;
    }
    if (!gcoap_pkt_set_payload(&pdu, buf + len, sizeof(buf) - len)) {
        puts("Error setting CoAP payload");
        return 1;
    }
    memcpy(pdu.payload, COAP_MSG, sizeof(COAP_MSG));
    len += sizeof(COAP_MSG);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UNDEF);
    if (res <= 0) {
        puts("Error sending CoAP request");
        return 1;
    }
    return 0;
}