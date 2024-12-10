#include "time_units.h"
#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP   "47.102.103.1"
#define COAP_PORT        5683
#define COAP_URI_PATH    "/coap/test"
#define COAP_PAYLOAD     "Hello, COAP Cloud"
#define RECONNECT_DELAY  (1U * US_PER_SEC)

static sock_udp_ep_t remote;

void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int init_coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON); // Set Confirmable

    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    coap_hdr_set_payload_len(pdu.hdr, strlen(COAP_PAYLOAD));

    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = COAP_PORT;

    while (true) {
        res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res >= 0) {
            puts("Request sent");
            break;
        } else {
            printf("Failed to send request: %ld\n", res);
            xtimer_usleep(RECONNECT_DELAY);
        }
    }

    return res >= 0 ? 0 : -1;
}

int main(void) {
    evtimer_msg_t my_evtimer;
    mbox_t my_mbox;
    
    mbox_init(&my_mbox, NULL);
    evtimer_msg_init(&my_evtimer);

    if (init_coap_request() < 0) {
        puts("Failed to initialize CoAP request.");
        return 1;
    }

    while(1) {
        evtimer_msg_timeout(&my_evtimer, &my_mbox);
        msg_t msg;
        mbox_recv(&my_mbox, &msg);
    }

    return 0;
}