
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define COAP_PDU_BUF_SIZE (128)

static uint8_t buf[COAP_PDU_BUF_SIZE];
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        char *class_str = (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS) ? "Success" : "Error";
        printf("Response Code: %s\n", class_str);
    }
}

static void send_coap_request(void)
{
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, COAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void)
{
    puts("CoAP Client starting");

    /* Initialize the gcoap module */
    gcoap_init();

    /* Send the CoAP request every 5 seconds */
    while (1) {
        send_coap_request();
        xtimer_sleep(5);
    }

    return 0;
}
