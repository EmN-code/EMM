#include <inttypes.h>
#include <net/af.h>
#include <net/sock.h>
#include <coap.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, 
                          const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout for msg ID %02u\n", memo->id);
        return;
    }

    if (pdu == NULL) {
        printf("remote responded with empty ack\n");
        return;
    }

    unsigned code = coap_get_code(pdu);
    printf("response code: %u\n", code);
    if (code == COAP_CODE_205_CONTENT) {
        char content[64];
        size_t payload_len = coap_get_payload(pdu, (uint8_t **)&content);
        printf("Response payload: %.*s\n", (int)payload_len, content);
    }
}

int main(void) {
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    ssize_t bytes = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (bytes < 0) {
        printf("gcoap_req_init failed\n");
        return 1;
    }

    const char* payload = "Hello, COAP Cloud";
    ssize_t hdrlen = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if(hdrlen < 0){
        printf("coap_opt_finish failed\n");
        return 1;
    }

    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    bytes = gcoap_req_send(buf, hdrlen + pdu.payload_len, &remote,
                           _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if(bytes <= 0){
        printf("gcoap_req_send failed\n");
    } else {
        printf("gcoap_req_send success\n");
    }

    return 0;
}