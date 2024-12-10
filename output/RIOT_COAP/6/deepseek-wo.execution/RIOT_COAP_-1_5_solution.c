
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Add the payload manually since 'coap_add_payload' is not allowed
    size_t payload_len = strlen(PAYLOAD);
    if (pdu.payload_len + payload_len > sizeof(buf)) {
        printf("Payload too large\n");
        return -1;
    }
    memcpy(pdu.payload, PAYLOAD, payload_len);
    pdu.payload_len += payload_len;

    // Set the remote IP address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return -1;
    }

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
