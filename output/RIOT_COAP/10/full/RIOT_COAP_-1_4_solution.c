#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <kernel_defines.h>
#include <arpa/inet.h>

#define COAP_SERVER_IP   "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH    "/coap/test"
#define COAP_MSG        "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP)
    {
        printf("Received response
");
    }
    else
    {
        printf("Request not successful
");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t ret;
    char *payload = COAP_MSG;
    size_t payload_len = strlen(payload);

    /* Define remote endpoint structure */
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT
    };

    /* Convert IP address */
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return -1;
    }

    /* Prepare CoAP request buffer */
    ret = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (ret < 0) {
        printf("Error: unable to initialize CoAP request\n");
        return -1;
    }

    /* Add payload marker */
    if (coap_opt_add_payload_marker(&pdu) < 0) {
        printf("Error: unable to add payload marker\n");
        return -1;
    }

    /* Copy payload to buffer */
    memcpy(coap_payload(&pdu), payload, payload_len);
    pdu.payload_len = payload_len;

    /* Keep trying to send until successful */
    while (1) {
        ret = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (ret < 0) {
            printf("Error: unable to send CoAP request, trying again...\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}