#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define COAP_METHOD_POST 2
#define GCOAP_SOCKET_TYPE_UDP 0

static ssize_t _resp_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, 
                             const sock_udp_ep_t *remote)
{
    // Response handling logic.
    if (pdu == NULL) {
        printf("Response is NULL\n");
        return -1;
    }

    printf("Response received\n");
    return 0;
}

int main(void)
{
    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t res;
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT
    };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE));
    pdu.payload_len = sizeof(MESSAGE);

    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error sending CoAP request, retrying...\n");
            xtimer_sleep(1);
        } else {
            printf("CoAP message sent successfully\n");
            break;
        }
    }
    return 0;
}