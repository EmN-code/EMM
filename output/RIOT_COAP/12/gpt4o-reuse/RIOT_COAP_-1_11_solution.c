
#include "gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "fmt.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        char resp_buf[CONFIG_GCOAP_PDU_BUF_SIZE];
        size_t len = gcoap_get_payload(pdu, (uint8_t **)&resp_buf);
        resp_buf[len] = '\0';
        printf("Received response: %s\n", resp_buf);
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    ssize_t len = gcoap_finish(&pdu, pdu.payload_len, COAP_FORMAT_TEXT);
    if (len < 0) {
        puts("Error: cannot finish CoAP request");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    xtimer_sleep(5); // Wait for response

    return 0;
}
