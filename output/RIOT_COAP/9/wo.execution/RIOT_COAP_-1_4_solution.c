#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout\n");
        return;
    }
    if (pdu->code == COAP_CODE_205_CONTENT) {
        printf("success: received: %u\n", (unsigned) (pdu->payload_len));
    }
    else {
        printf("error: code %u\n", pdu->code);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    int res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("inet_pton failed\n");
        return 1;
    }

    ssize_t bytes;

    // Initialize a CoAP packet
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) == -1) {
        printf("gcoap_req_init failed\n");
        return 1;
    }

    // Set the payload
    char payload[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, sizeof(payload) - 1);
    coap_hdr_set_payload_len(&pdu, sizeof(payload) - 1);

    // Send the CoAP request
    bytes = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, &_response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes <= 0) {
        printf("gcoap_req_send failed\n");
        return 1;
    }

    printf("Request sent\n");

    return 0;
}