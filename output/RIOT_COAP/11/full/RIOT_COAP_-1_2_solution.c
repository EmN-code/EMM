#include "net/gcoap.h"
#include "netinet/in.h"
#include "net/af.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#include <stdio.h>
#include <string.h>

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                             const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu != NULL) {
        printf("Response received: %u\n", coap_get_code(pdu));
    }
    else {
        printf("No response received\n");
    }
    return 0;
}

int main(void)
{
    /* Endpoint for the CoAP server */
    sock_udp_ep_t server = { .family = AF_INET, .port = COAP_SERVER_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, COAP_SERVER_ADDR) == NULL) {
        puts("Error parsing server address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);

    const char *data = "Hello, COAP Cloud";
    memcpy(pdu.payload, data, strlen(data));
    size_t len = gcoap_finish(&pdu, strlen(data), COAP_FORMAT_TEXT);

    int retry_count = 0;
    int max_retries = 5;
    while (1) {
        int res = gcoap_req_send(buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (res >= 0) {
            printf("Request sent successfully\n");
            break;
        }
        else {
            printf("Error sending request\n");
            retry_count++;
            if (retry_count >= max_retries) {
                printf("Max retries reached. Exiting.\n");
                break;
            }
            printf("Retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }

    return 0;
}