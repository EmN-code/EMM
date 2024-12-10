#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "arpa/inet.h"

#define GCOAP_PORT_DEF 5683 // Defining CoAP port here

static const char *REMOTE_ADDR = "47.102.103.1";
static const char *COAP_URI_PATH = "/coap/test";

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu) {
        printf("[INFO] Response received: %s\n", (char *)pdu->payload);
    }
    else {
        printf("[ERROR] No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;
    sock_udp_ep_t server;

    // Network settings
    server.family = AF_INET;
    server.netif = SOCK_ADDR_ANY_NETIF;
    server.port = GCOAP_PORT_DEF;
    inet_pton(AF_INET, REMOTE_ADDR, &server.addr.ipv6[12]); // Allocate IPv4 mapped address

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    if (len < 0) {
        printf("[ERROR] Unable to initialize request\n");
        return 1;
    }

    // Set payload
    if (coap_opt_finish(&pdu, len, COAP_OPT_FINISH_PAYLOAD) < 0) {
        printf("[ERROR] Error finishing CoAP options\n");
        return 1;
    }

    memcpy(pdu.payload, "Hello, COAP Cloud", 17);

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("[ERROR] Failed to send CoAP request\n");
        return 1;
    }

    printf("[INFO] CoAP request sent\n");

    return 0;
}