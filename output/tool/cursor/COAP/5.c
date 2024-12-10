#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IPv4 "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout");
        return;
    }
    if (pdu->hdr->code == COAP_CODE_204) {
        puts("Success");
    } else {
        printf("Error: %u\n", pdu->hdr->code);
    }
}

static void send_coap_request(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_add_uint(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    bytes = gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL);
    if (bytes <= 0) {
        puts("Error sending request");
    }
}

int main(void)
{
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, SERVER_IPv4);

    while (1) {
        send_coap_request();
        xtimer_sleep(10); // Send request every 10 seconds
    }

    return 0;
}