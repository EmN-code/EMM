#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdint.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#include <stdlib.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define POST_DATA "Hello, COAP Cloud"

static sock_udp_ep_t remote;

void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response from a CoAP server
    (void)pdu;
    (void)remote;
    printf("Response handled\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    int rc;

    while (1) {
        memset(&remote, 0, sizeof(sock_udp_ep_t));
        remote.family = AF_INET;
        rc = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
        if (rc != 1) {
            puts("inet_pton failed");
            xtimer_sleep(1);
            continue;
        }
        remote.port = SERVER_PORT;
        
        gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
        coap_payload(&pdu, (uint8_t *)POST_DATA, strlen(POST_DATA));
        len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
        len = gcoap_payload(&pdu, &buf[len]);

        ssize_t res;
        res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("gcoap_req_send failed: %d\n", res);
            xtimer_sleep(1);
            continue;
        }

        printf("CoAP request sent successfully\n");
        break;
    }

    return 0;
}