// The following C code shows how to send a CoAP POST request in RIOT OS for ESP32, 
// which will send "Hello, COAP Cloud" to a CoAP server with specified IP and port.

#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "stdio.h"
#include "mutex.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "assert.h"

#define EMQX_SERVER_IP   "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH    "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define PAYLOAD "Hello, COAP Cloud"

static gcoap_listener_t _listener;
static msg_t main_msg_queue[8];

static void _resp_handler(const gcoap_request_memo_t *memo,
                          coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout for msg ID %u\n", coap_get_id(pdu));
        return;
    }
    unsigned code = coap_get_code(pdu);
    printf("Response %u.%u for msg ID %u\n",
           coap_get_code_class(code),
           coap_get_code_detail(code),
           coap_get_id(pdu));
}

void send_coap_post(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));

    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE,
                   COAP_METHOD_POST, EMQX_URI_PATH);

    size_t payload_len = strlen(PAYLOAD);
    memcpy(pdu.payload, PAYLOAD, payload_len);
    pdu.payload_len = payload_len;

    gcoap_req_send(&buf[0], pdu.hdrlen + pdu.payload_len, &remote,
                   _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    msg_init_queue(main_msg_queue, 8);
    printf("RIOT started. Sending CoAP POST...\n");

    ztimer_sleep(ZTIMER_SEC, 1); // Delay to ensure everything is initialized

    send_coap_post();

    while (1) {
        // Keep the main thread running; handle other tasks/events as necessary.
        ztimer_sleep(ZTIMER_SEC, 10);
    }

    return 0;
}