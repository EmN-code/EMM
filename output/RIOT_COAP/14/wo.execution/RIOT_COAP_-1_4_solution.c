#include <stdio.h>
#include <stddef.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/nanocoap/cache.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <esp_system.h>

// Constants for CoAP
#define CONFIG_GCOAP_PDU_BUF_SIZE 256
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define COAP_OPT_FINISH_NONE 0

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static coap_pkt_t pdu;

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *ep)
{
    // Handle response here
    (void)memo;
    (void)pdu;
    (void)ep;
}

int main(void)
{
    // Setup remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("inet_pton: Error\n");
        return -1;
    }

    // Initialize the CoAP PDU
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) == 0) {
        printf("gcoap_req_init: Error\n");
        return -1;
    }

    // Add payload
    const char *payload = "Hello, COAP Cloud";
    if (coap_opt_add_payload(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, (uint8_t *)payload, strlen(payload)) == 0) {
        printf("coap_opt_add_payload: Error\n");
        return -1;
    }

    // Finish packet
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) == 0) {
        printf("coap_opt_finish: Error\n");
        return -1;
    }

    // Send request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("gcoap_req_send: Error\n");
        return -1;
    }

    return 0;
}