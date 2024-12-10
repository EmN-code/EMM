#include <xtimer.h>
#include <net/gcoap.h>
#include <msg.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static gcoap_listener_t _listener;
static gcoap_request_memo_t _req_memo;
static uint8_t _req_buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received: %d\n", coap_get_code(pdu));
}

void send_coap_request(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = SERVER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    gcoap_req_init(&pdu, _req_buf, sizeof(_req_buf), COAP_METHOD_POST, URI_PATH);
    char payload[] = "Hello, COAP Cloud";
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, payload, sizeof(payload));
    pdu.payload_len = sizeof(payload);

    gcoap_req_send(_req_buf, pdu.payload_len, &remote, _resp_handler, &_listener, GCOAP_SOCKET_TYPE_UDP);
    printf("Request sent to %s\n", SERVER_IP);
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue));
    gcoap_register_listener(&_listener);

    xtimer_sleep(1);
    send_coap_request();

    return 0;
}