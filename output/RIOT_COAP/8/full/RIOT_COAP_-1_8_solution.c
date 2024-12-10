#include "xtimer.h"
#include "thread.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "timex.h"
#include "msg.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static const uint8_t payload[] = COAP_PAYLOAD;

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t* remote, void* context) {
    (void)context;
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("GCOAP: Timeout for request.\n");
        return;
    }
    printf("GCOAP: Response %u bytes\n", pdu->payload_len);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Set remote address
    while (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        xtimer_sleep(1);
    }
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    // Initialise CoAP message
    ssize_t init = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (init < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    memcpy(pdu.payload, payload, sizeof(payload));
    pdu.payload_len = sizeof(payload);

    // Attempt to send the CoAP request with robust error handling
    while (1) {
        ssize_t res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res >= 0) {
            printf("GCOAP: Request sent successfully\n");
            break;
        }
        else {
            printf("Failed to send request: %d\n", (int)res);
            xtimer_sleep(1);
        }
    }

    return 0;
}