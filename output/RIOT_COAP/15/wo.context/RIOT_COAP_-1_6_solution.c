#include <xtimer.h>
#include <string.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT /* Default CoAP port: 5683 */
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    int res;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    while (1) {
        if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
            printf("Error converting IP address\n");
            xtimer_sleep(1);
            continue;
        }
        printf("IP address converted successfully\n");
        break;
    }

    while (1) {
        res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
        if (res < 0) {
            printf("Error initializing request\n");
            xtimer_sleep(1);
            continue;
        }

        memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
        pdu.payload_len = strlen(MESSAGE);

        if (gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
            printf("Failed to send CoAP request\n");
            xtimer_sleep(1);
            continue;
        }
        printf("CoAP request sent successfully\n");
        break;
    }

    return 0;
}