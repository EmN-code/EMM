
#include <stdio.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <time_units.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Response handling logic
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Convert IP address string to byte representation
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    // Initialize CoAP request
    result = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (result < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the CoAP request
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Finish CoAP options
    result = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (result < 0) {
        printf("Failed to finish CoAP options\n");
        return 1;
    }

    // Send the CoAP request
    result = gcoap_req_send(buf, result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}
