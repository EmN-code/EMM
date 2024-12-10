#include "mma8x5x.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "mutex.h"
#include "evtimer_mbox.h"
#include "stdatomic.h"
#include "stdio.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "evtimer_msg.h"
#include "net/emcute.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "stddef.h"
#include "msg.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "timex.h"
#include "mma8x5x_params.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
        char *payload = (char *)pdu->payload;
        printf("Payload: %s\n", payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int rc;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    rc = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    if (rc <= 0) {
        printf("Failed to convert server address\n");
        return 1;
    }

    while (1) {
        rc = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        if (rc == 0) {
            printf("CoAP request initialized\n");
        } else {
            printf("Failed to initialize CoAP request\n");
            continue;
        }

        memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
        rc = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
        if (rc < 0) {
            printf("Error finishing CoAP options\n");
            continue;
        }

        ssize_t bytes_sent = gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (bytes_sent <= 0) {
            printf("Failed to send CoAP request. Retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            printf("CoAP request sent. Waiting for response...\n");
            break;
        }
    }

    return 0;
}