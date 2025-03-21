
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "esp_system.h"
#include "mma8x5x_params.h"
#include "mutex.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_wifi.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stddef.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define MESSAGE_CONTENT "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the CoAP request
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, MESSAGE_CONTENT, sizeof(MESSAGE_CONTENT));
    len += sizeof(MESSAGE_CONTENT);

    // Send the CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}
