#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "string.h"
#include "msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "architecture.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "dht_params.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "time_units.h"
#include "mbox.h"
#include "timex.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

int main(void) {
    network_t network;
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    int rc;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    while (1) {
        rc = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (rc != 1) {
            printf("inet_pton failed\n");
            xtimer_sleep(1);
            continue;
        }

        gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
        memcpy(pdu.payload, COAP_MSG, sizeof(COAP_MSG) - 1);
        gcoap_opt_finish(&pdu, sizeof(COAP_MSG) - 1);

        rc = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc <= 0) {
            printf("Error sending request\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("CoAP POST request sent\n");
            break;
        }
    }

    return 0;
}