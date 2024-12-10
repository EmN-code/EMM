#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>

#define SERVER_ADDR    "47.102.103.1"
#define SERVER_PORT    5683
#define COAP_PATH      "/coap/test"
#define MESSAGE        "Hello, COAP Cloud"
#define RECONNECT_INTERVAL (1U * US_PER_SEC)

static ssize_t _resp_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len)
{
    ssize_t ret = coap_reply_simple(pdu, COAP_CODE_CONTENT, buf, len, COAP_FORMAT_TEXT, (const uint8_t*)MESSAGE, strlen(MESSAGE));
    return ret;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;
    sock_udp_ep_t remote;

    remote.family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);
    remote.port = SERVER_PORT;

    while (1) {
        len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_PATH);
        coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

        if (len == 0) {
            puts("gcoap_req_init failed");
            xtimer_sleep(1);
            continue;
        }

        strncpy((char *)pdu.payload, MESSAGE, pdu.payload_len);

        if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
            puts("gcoap_req_send failed, retrying...");
            xtimer_sleep(1);
            continue;
        } else {
            puts("CoAP request sent successfully");
            break;
        }
    }

    return 0;
}
