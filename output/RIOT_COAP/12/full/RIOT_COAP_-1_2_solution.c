#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"

#define COAP_PORT 5683
#define REMOTE_ADDR "47.102.103.1"
#define COAP_PATH "/coap/test"

static ssize_t _resp_handler(coap_pkt_t* pdu, const sock_udp_ep_t* remote, void* context) {
    (void)pdu;
    (void)remote;
    (void)context;
    printf("Received response\n");
    return 0;
}

int main(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize CoAP packet
    int init_res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_PATH);
    if (init_res < 0) {
        printf("Failed to initialize request\n");
        return init_res;
    }

    // Add payload
    coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Convert IP address
    while (inet_pton(AF_INET, REMOTE_ADDR, &remote.addr.ipv4) != 1) {
        printf("IP address conversion failed, retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    remote.port = COAP_PORT;

    ssize_t res = -1;

    // Send CoAP request
    while ((res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP)) <= 0) {
        printf("Failed to send request, retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    printf("Request sent successfully\n");
    return 0;
}