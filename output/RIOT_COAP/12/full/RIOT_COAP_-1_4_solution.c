#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <arpa/inet.h>

#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define POST_MESSAGE "Hello, COAP Cloud"

static sock_udp_ep_t remote;
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(coap_pkt_t *pdu, sock_udp_ep_t *remote)
{
    (void)remote;
    printf("Response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
}

int main(void)
{
    // Initialize remote IP and port
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = EMQX_SERVER_PORT;

    while (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address format. Retrying...\n");
        xtimer_sleep(1);
    }

    size_t len;
    coap_pkt_t pdu;
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT) == 0) {
        printf("Failed to add format option.\n");
        return -1;
    }

    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    len += coap_payload(pdu.hdr, POST_MESSAGE, strlen(POST_MESSAGE));

    // Repeatedly send request until successful
    while (1) {
        ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (res < 0) {
            printf("Failed to send request. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Request sent successfully.\n");
            break;
        }
    }

    return 0;
}