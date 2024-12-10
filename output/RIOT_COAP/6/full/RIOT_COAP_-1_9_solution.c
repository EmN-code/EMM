#include "ztimer.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "string.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"
#define BUFFER_SIZE 128

void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int rc = 0;

    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    while(1) {
        rc = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (rc != 1) {
            printf("IP address conversion failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
        ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

        if (len < 0) {
            printf("Packet build failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        rc = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("Failed to send request\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        printf("Request sent successfully\n");
        break;
    }

    return 0;
}