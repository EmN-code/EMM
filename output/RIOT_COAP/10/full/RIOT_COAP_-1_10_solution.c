#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "bool.h"
#include "stdio.h"
#include "stddef.h"
#include "netinet/in.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "arpa/inet.h"

#define SERVER_IP            "47.102.103.1"
#define SERVER_PORT          5683
#define EMQX_URI_PATH        "/coap/test"
#define PAYLOAD              "Hello, COAP Cloud"
#define MAX_ATTEMPTS         5

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) 
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int rc;
    
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: converting IP address\n");
        return -1;
    }
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    memcpy(&pdu.payload[0], PAYLOAD, strlen(PAYLOAD));
    unsigned len = gcoap_get_request_length(&pdu);

    int attempts = 0;
    int success = 0;

    while (attempts < MAX_ATTEMPTS) {
        rc = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc <= 0) {
            printf("Failed to send request, attempt %d\n", attempts + 1);
            xtimer_sleep(1);  // retry after 1 second
            attempts++;
        } else {
            printf("Request sent\n");
            success = 1;
            break;
        }
    }

    if (!success) {
        printf("Failed to send request after %d attempts\n", MAX_ATTEMPTS);
        return -1;
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) 
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout on request\n");
        return;
    }
    if (memo->state == GCOAP_MEMO_ERR) {
        printf("Error in response\n");
        return;
    }
    printf("Received response\n");
}