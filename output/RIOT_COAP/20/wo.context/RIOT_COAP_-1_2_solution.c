#include <net/sock/udp.h> // Ensure correct networking capabilities
#include <gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define URI_PATH "/coap/test"

static sock_udp_ep_t server_address = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response
    (void) memo;
    (void) pdu;
    (void) remote;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    // Initialize packet
    int result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        puts("Failed to initialize request");
        return 1;
    }

    // Set payload
    const char* payload = "Hello, COAP Cloud";
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_payload(&pdu, (uint8_t*)payload, strlen(payload));

    // Finalize packet
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Set server IP
    inet_pton(AF_INET, SERVER_IP, &server_address.addr.ipv4);

    // Send request
    if (gcoap_req_send(buf, len, &server_address, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == -ENOMEM) {
        puts("Failed to send request");
    } else {
        puts("Request sent");
    }

    return 0;
}