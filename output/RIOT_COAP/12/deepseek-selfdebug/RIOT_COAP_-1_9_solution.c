
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"  // Include this header for inet_pton
#include "shell.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *resp_buf = (char *)pdu->payload;
    size_t resp_len = pdu->payload_len;

    printf("Received CoAP response: %.*s\n", (int)resp_len, resp_buf);
}

static int _send_coap_post(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);

    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    ssize_t len = gcoap_finish(&pdu, payload_len, COAP_FORMAT_TEXT);
    if (len < 0) {
        puts("Error in gcoap_finish");
        return -1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "coap_post", "Send a CoAP POST request", _send_coap_post },
    { NULL, NULL, NULL }
};

int main(void)
{
    puts("CoAP client example");

    /* start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
