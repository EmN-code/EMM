#include <net/sock/udp.h>
#include <net/ip.h>
#include <net/gcoap.h>
#include <kernel_defines.h>

#define GCOAP_PDU_BUF_SIZE      (128)
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; // Unused parameter
    if (pdu->hdr->code == COAP_CODE_204) {
        puts("Success: CoAP request completed");
    }
    else {
        puts("Failure: Could not complete CoAP request");
    }
}

int main(void) {
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int result;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: Unable to parse server IP");
        return -1;
    }
    remote.port = 5683;

    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    coap_hdr_set_total_length(pdu.hdr, coap_packet_build_payload_offset(pdu.hdr, &pdu) + strlen(payload));

    ssize_t len = gcoap_req_send(buf, pdu.hdr_len+strlen(payload), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        puts("Error: Failed to send CoAP request");
        return -1;
    }

    puts("CoAP request sent");

    return 0;
}