#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "mutex.h"
#include "esp_system.h"
#include "esp_wifi.h"

#define SERVER_PORT 5683
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;

static ssize_t _resp_handler(coap_pkt_t* pdu, void* ctx, sock_udp_ep_t* ep) {
    (void)pdu;
    (void)ctx;
    (void)ep;
    printf("Response received\n");
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Unable to convert IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    if(gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}