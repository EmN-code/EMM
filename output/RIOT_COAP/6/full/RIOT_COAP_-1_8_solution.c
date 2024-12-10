#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "msg.h"
#include "timex.h"
#include "netinet/in.h"
#include "stdatomic.h"
#include "dht_params.h"
#include "evtimer_msg.h"
#include "net/emcute.h"
#include "esp_system.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "esp_wifi.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;
static char response[1024];

void _resp_handler(const gcoap_response_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu->payload_len) {
        memcpy(response, (char *)pdu->payload, pdu->payload_len);
        response[pdu->payload_len] = '\0';
        printf("Response: %s\n", response);
    }
    else {
        printf("No response payload\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    while (1) {
        int result;
        result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (result != 1) {
            printf("Invalid server IP address\n");
            continue;
        } else {
            printf("Server IP set successfully\n");
            break;
        }
    }
    
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);
    
    while (1) {
        ssize_t gcoap_ret;
        gcoap_ret = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (gcoap_ret > 0) {
            printf("CoAP request sent successfully\n");
            break;
        } else {
            printf("Sending CoAP request failed\n");
            xtimer_sleep(1);
        }
    }

    return 0;
}