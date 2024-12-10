
#include <timex.h>
#include <evtimer_mbox.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        puts("Request succeeded");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the remote endpoint with the server's IP address
    if (sock_udp_str2ep(&remote, SERVER_IP) < 0) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Initialize the CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) < 0) {
        puts("Error initializing CoAP request");
        return 1;
    }

    // Add the payload to the CoAP request
    size_t payload_len = strlen(MESSAGE);
    memcpy(pdu.payload, MESSAGE, payload_len);
    pdu.payload_len = payload_len;

    // Finish the CoAP request
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        puts("Error finishing CoAP request");
        return 1;
    }

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
