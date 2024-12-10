#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <net/gcoap.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static ssize_t _resp_handler(coap_pkt_t* pdu, void* context) {
    (void)context;
    if (pdu->hdr->code == COAP_CODE_205_CONTENT) {
        printf("Received valid response\n");
    } else {
        printf("Error in response\n");
    }
    return 0;
}

int main(void) {
    uint8_t buf[256];   // buffer to hold the request
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Initialize the CoAP request
    coap_pkt_t pdu;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("error: unable to parse destination address");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_pkt_append_payload(&pdu, (uint8_t*)MESSAGE, strlen(MESSAGE));

    // Send the CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("error: unable to send request");
        return -1;
    }

    return 0;
}
