#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "esp_wifi.h"

#define COAP_URI_PATH "/coap/test"
#define COAP_BUFFER_SIZE 128

void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote; /* not used */
    if (pdu == NULL) {
        puts("Error receiving response");
        return;
    }

    printf("Response: %d.%d\n", coap_get_code_class(pdu), coap_get_code_detail(pdu));
}

int main(void)
{
    /* set destination address */
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse destination address");
        return -1;
    }

    /* setup buffer and packet */
    uint8_t buf[COAP_BUFFER_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, COAP_BUFFER_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    /* add CoAP options */
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT) < 0) {
        puts("Error: unable to add format option");
        return -1;
    }

    /* add CoAP payload */
    const char *data = "Hello, COAP Cloud";
    if (coap_opt_add_payload_marker(&pdu) < 0) {
        puts("Error: unable to add payload marker");
        return -1;
    }

    size_t payload_len = strlen(data);
    if (coap_payload(&pdu) == NULL || payload_len > coap_opt_need_space(&pdu, payload_len)) {
        puts("Error: insufficient space for payload");
        return -1;
    }
    memcpy(coap_payload(&pdu), data, payload_len);
    coap_opt_finish(&pdu, payload_len, 0);

    ssize_t res;
    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("gcoap_req_send() failed with %d\n", (int)res);
            ztimer_sleep(ZTIMER_USEC, 1U * US_PER_SEC); // Retry after 1 second
            continue;
        } else {
            break;
        }
    }

    return 0;
}
