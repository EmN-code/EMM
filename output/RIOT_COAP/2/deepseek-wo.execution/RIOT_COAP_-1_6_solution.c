
#include <net/gcoap.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/emcute.h>
#include <msg.h>
#include <stdbool.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <stdatomic.h>
#include <esp_wifi.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define GCOAP_PDU_BUF_SIZE (128)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Add payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(&buf[pdu.payload_len], payload, payload_len);
    pdu.payload_len += payload_len;

    // Finish CoAP options
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
