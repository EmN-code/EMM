#include <string.h>  // Correct header file for memcpy
#include <net/sock/util.h> // For `inet_pton` and related network utilities
#include <net/gcoap.h> // Correct header file for CoAP related utilities like gcoap_response

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu != NULL) {
        printf("Response received with code %u\n", coap_get_code(pdu));
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    const char *uri_path = "/coap/test";
    const char *payload = "Hello, COAP Cloud";
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    
    remote.family = AF_INET;
    remote.port = 5683;
    
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }
    
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, uri_path);
    memcpy(coap_get_payload(&pdu), payload, strlen(payload));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    ssize_t bytes = gcoap_req_send(buf, coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        printf("Error sending request\n");
        return 1;
    }
    printf("Request sent\n");
    return 0;
}