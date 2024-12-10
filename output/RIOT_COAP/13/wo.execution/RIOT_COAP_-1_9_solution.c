#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_REQUEST_MSG "Hello, COAP Cloud"
#define GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo,
                          coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;

    puts("Response received");
}

int main(void) {
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    size_t len;
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    /* Prepare the remote endpoint */
    remote.family = AF_INET;
    remote.port = COAP_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    /* Initialize the CoAP packet */
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_REQUEST_MSG, sizeof(COAP_REQUEST_MSG));
    len += sizeof(COAP_REQUEST_MSG);

    /* Send the CoAP request */
    if(gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send request");
    }
    return 0;
}