#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "net/emcute.h"

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_RECEIVED) {
        printf("Response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    static sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: invalid server IPv4 address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    if (gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    return 0;
}