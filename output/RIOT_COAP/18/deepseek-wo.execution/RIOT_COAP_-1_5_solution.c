
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <mutex.h>
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
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Response handler logic here
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Set the payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Finish the CoAP options
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Convert the IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}