#include <string.h> // For memcpy and memset
#include <net/ipv6/addr.h>
#include <net/sock/udp.h> // For UDP definitions

#define CONFIG_GCOAP_PDU_BUF_SIZE      (128)

static const char *EMQX_URI_PATH = "/coap/test";
static const char *EMQX_SERVER_IP = "47.102.103.1";

// Function prototypes
void coap_request(void);

// Main function
int main(void) {
    coap_request();
    return 0;
}

void coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .port = 5683 };

    // Convert IP address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv6) != 1) {
        puts("Error: unable to convert IP address");
        return;
    }

    // Create CoAP request
    coap_pkt_t pdu;
    ssize_t res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (res < 0) {
        puts("Error initializing CoAP request");
        return;
    }

    // Add payload to request
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(pdu.payload, payload, payload_len);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_hdr_set_id(pdu.hdr, coap_get_id());  

    // Send CoAP request
    res = gcoap_req_send(buf, pdu.hdr->len + payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        puts("Error: sending CoAP request failed");
    } else {
        puts("CoAP message sent successfully");
    }
}
