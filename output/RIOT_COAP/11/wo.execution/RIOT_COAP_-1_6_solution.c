#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mutex.h"

// Simplified RIOT API error fix

static sock_udp_ep_t remote = {.family = AF_INET, .port = 5683};

static void _resp_handler(unsigned req_state, coap_pkt_t* pkt, const sock_udp_ep_t* ep, void* context) {
    (void)context;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize CoAP request
    unsigned msg_type = coap_get_type(COAP_METHOD_POST, buf, sizeof(buf), &remote, gcoap_socket_type_udp);
    gcoap_req_init(&pdu, buf, sizeof(buf), msg_type, "/coap/test");

    // Set CoAP payload
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Convert string IP address to byte representation
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    
    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res <= 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent");

    return 0;
}