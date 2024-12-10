#include <thread_config.h>
#include <net/nanocoap/cache.h>
#include <gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response (for simplicity, we'll just acknowledge the reception here)
    if (pdu->hdr->code == COAP_CODE_205) {
        printf("Success, message sent!\n");
    } else {
        printf("Failed to send message\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = SERVER_PORT
    };

    // Set the server's IPv6 address
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE));
    pdu.payload_len = sizeof(MESSAGE);

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload_len, &remote, resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    while(1) {
        // Loop here to keep the program running
        // In a real-world scenario, you might handle other tasks or enter low-power mode
        thread_yield();
    }

    return 0;
}