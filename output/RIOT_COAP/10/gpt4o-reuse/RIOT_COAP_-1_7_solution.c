
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "shell.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static ssize_t _coap_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;
    if (pdu->hdr->code == COAP_CODE_205_CONTENT) {
        printf("Received response: %.*s\n", (int)pdu->payload_len, (const char *)pdu->payload);
    } else {
        printf("Error in response\n");
    }
    return 0;
}

static int _send_coap_post(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    gcoap_req_send(buf, len, &remote, _coap_handler, NULL);
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "coap_post", "Send a CoAP POST request", _send_coap_post },
    { NULL, NULL, NULL }
};

int main(void)
{
    gcoap_init();
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
