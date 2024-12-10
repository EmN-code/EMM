#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "esp_system.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH    "/coap/test"
#define USE_IPV6    0 // Set to 1 if using IPv6 address

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, sock_udp_ep_t *remote)
{
    printf("Response received!\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    #if USE_IPV6
    char ipv6_addr_str[] = "[fe80::abcd:1234]"; // Example IPv6 address
    int ret = inet_pton(AF_INET6, ipv6_addr_str, &(remote.addr.ipv6));
    #else
    int ret = sock_str2addr(SERVER_IP, &remote.addr.ipv4, AF_INET);
    #endif

    while (ret <= 0) {
        printf("Invalid address. Retrying...\n");
        xtimer_sleep(1);
        #if USE_IPV6
        ret = inet_pton(AF_INET6, ipv6_addr_str, &(remote.addr.ipv6));
        #else
        ret = sock_str2addr(SERVER_IP, &remote.addr.ipv4, AF_INET);
        #endif
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (coap_payload(&pdu)) {
        printf("Payload set as message\n");
    }
    strcpy((char *)coap_payload(&pdu), "Hello, COAP Cloud");
    coap_set_payload(&pdu, (const uint8_t*)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("Failed to send, retrying...\n");
            xtimer_sleep(1);
        } else {
            printf("CoAP message sent\n");
            break;
        }
    }

    return 0;
}