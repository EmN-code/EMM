#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "msg.h"
#include "timex.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "netinet/in.h"

#define SERVER_PORT (5683)
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define COAP_METHOD_POST 0x02
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote, void *context) {
    (void)context;
    (void)remote;
    printf("Response received! State: %u\n", req_state);
    if (pdu == NULL) {
        puts("GCOAP Error: response was NULL");
    }
    else {
        printf("CoAP Response Code: %u\n", coap_get_code(pdu));
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT }; 
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("ERROR: unable to parse destination address\n");
        return 1;
    }

    int size = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (size < 0) {
        puts("GCOAP Error: Unable to initialize request");
    }
    else {
        const char* payload = "Hello, COAP Cloud";
        size += coap_payload(&pdu, (uint8_t *)payload, strlen(payload));

        int result = gcoap_req_send(buf, size, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (result <= 0) {
            puts("GCOAP Error: Unable to send request");
        }
        else {
            puts("Request successfully sent");
        }
    }

    return 0;
}