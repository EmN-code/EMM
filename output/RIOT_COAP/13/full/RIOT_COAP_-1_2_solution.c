#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "ipv6/addr.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define COAP_METHOD COAP_METHOD_POST

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout received!");
        return;
    }
    puts("Response received successfully!");
}

typedef struct {
    sock_udp_ep_t remote;
} network_context_t;

int init_connection(network_context_t *network_context) {
    network_context->remote.family = AF_INET6;
    if (ipv6_addr_from_str((ipv6_addr_t *)&network_context->remote.addr.ipv6, COAP_SERVER_ADDR) == NULL) {
        puts("Error: Unable to decode IPv6 address");
        return -1;
    }
    network_context->remote.port = COAP_SERVER_PORT;
    return 0;
}

int send_coap_request(const network_context_t *network_context) {
    uint8_t buffer[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;

    if (gcoap_req_init(&pdu, buffer, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, COAP_URI_PATH) == 0) {
        puts("Error initializing CoAP request");
        return -1;
    }
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    bytes = gcoap_finish(&pdu, (const uint8_t *)COAP_PAYLOAD, sizeof(COAP_PAYLOAD) - 1, COAP_FORMAT_NONE);
    if (bytes < 0) {
        puts("Error finishing CoAP request");
        return -1;
    }

    ssize_t len = gcoap_req_send(buffer, bytes, &network_context->remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        puts("Error sending CoAP Request");
        return -1;
    }

    return 0;
}

int main(void) {
    network_context_t network_context;
    int rc;

    while (1) {
        rc = init_connection(&network_context);
        if (rc != 0) {
            puts("Failed to establish connection. Retrying...");
            xtimer_sleep(1);
            continue;
        }

        rc = send_coap_request(&network_context);
        if (rc != 0) {
            puts("Failed to send CoAP request. Retrying...");
            xtimer_sleep(1);
        }
        else {
            puts("CoAP request sent successfully");
            break;
        }
    }

    return 0;
}