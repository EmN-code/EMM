#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#include <assert.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define TRY_INTERVAL 1
#define COAP_PDU_BUF_SIZE 128

static sock_udp_ep_t remote = {
    .family = AF_INET,
    .netif  = SOCK_ADDR_ANY_NETIF,
    .port   = COAP_SERVER_PORT
};

void setup_remote(sock_udp_ep_t *remote){
    inet_pton(AF_INET, COAP_SERVER_IP, &remote->addr);
}

void retry_network_connect() {
    while (1) {
        int rc = NetworkConnect();
        // Assuming NetworkConnect() returns 0 on success.
        if (rc != 0) {
            printf("Connect failed, retrying...\n");
            xtimer_sleep(TRY_INTERVAL);
            continue;
        } else {
            printf("Connected successfully\n");
            break;
        }
    }
}

int main(void) {
    uint8_t buf[COAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    int rc;

    setup_remote(&remote);

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, COAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    len += strlen(MESSAGE);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Attempt to send CoAP request, retry if needed
    retry_network_connect();
    rc = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (rc <= 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent\n");
    }
    return 0;
}

void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}