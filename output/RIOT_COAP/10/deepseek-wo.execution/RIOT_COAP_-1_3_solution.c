
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    // Handle response if needed
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Convert IP address string to byte representation
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize CoAP request
    result = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (result < 0) {
        puts("Error initializing CoAP request");
        return 1;
    }

    // Add payload to the CoAP request
    size_t payload_len = strlen(COAP_PAYLOAD);
    memcpy(pdu.payload, COAP_PAYLOAD, payload_len);
    pdu.payload_len = payload_len;

    // Finish the CoAP request
    result = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (result < 0) {
        puts("Error finishing CoAP request");
        return 1;
    }

    // Send the CoAP request
    result = gcoap_req_send(buf, result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
