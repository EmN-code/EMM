#include "xtimer.h"
#include "net/gcoap.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define POST_MESSAGE "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t res;
    uint8_t attempts = 0;

    /* Initialize the remote server endpoint */
    remote.family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_IP);
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    /* Retry until connection succeeds */
    while (1) {
        /* Initialize the CoAP request */
        if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) == NULL) {
            printf("Failed to initialize CoAP request\n");
            if (++attempts > 10) {
                printf("Max attempts reached. Exiting.\n");
                return -1;
            }
            xtimer_sleep(1);
            continue;
        }

        /* Set the payload */
        if (!coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE)) {
            printf("Failed to finish CoAP options\n");
            xtimer_sleep(1);
            continue;
        }
        memcpy(pdu.payload, POST_MESSAGE, strlen(POST_MESSAGE));
        pdu.payload_len = strlen(POST_MESSAGE);

        /* Send the request to the server */
        res = gcoap_req_send(pdu.payload, pdu.hdr.len + 1 + pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (res <= 0) {
            printf("Failed to send CoAP request (attempt %u)\n", attempts);
            xtimer_sleep(1);
            continue;
        }

        /* Success */
        printf("CoAP request sent: %s\n", POST_MESSAGE);
        break;
    }

    return 0;
}