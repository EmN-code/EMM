
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling logic here
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        // Handle IP conversion error
        return 1;
    }

    // Initialize the CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        // Handle request initialization error
        return 1;
    }

    // Add payload to the CoAP request
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        // Handle request send error
        return 1;
    }

    return 0;
}
