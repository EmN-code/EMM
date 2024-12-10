#include "time_units.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"

#define COAP_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    int res;
    char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = COAP_PORT;

    res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    res = gcoap_req_init(&pdu, (uint8_t *)buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (res < 0) {
        printf("Failed to initialize request\n");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD) - 1);
    pdu.payload_len = sizeof(PAYLOAD) - 1;

    ssize_t bytes_sent = gcoap_req_send((uint8_t *)buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send request\n");
        return 1;
    }

    printf("CoAP request sent successfully!\n");
    return 0;
}