#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"

#define PAYLOAD "Hello, COAP Cloud"
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout response\n");
        return;
    }
    printf("Response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Prepare the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t len = gcoap_finish(&pdu, strlen(PAYLOAD), COAP_FORMAT_TEXT);

    // Convert IP address string to binary
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.port = COAP_PORT;

    // Send the request
    ssize_t res = gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error: unable to send CoAP request\n");
        return -1;
    }

    puts("CoAP request sent\n");
    return 0;
}