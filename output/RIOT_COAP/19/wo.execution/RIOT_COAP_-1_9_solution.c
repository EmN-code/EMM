#include "msg.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "stdbool.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "dht.h"
#include "net/gcoap.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "net/emcute.h"
#include "timex.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "unistd.h"
#include "mma8x5x_params.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"

#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling logic
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}