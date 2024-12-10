#include <stdio.h> 
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>  

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

// Function to handle response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
   (void)memo;
   (void)remote;
   printf("Response received\n");
   if (pdu->payload_len) {
       printf("Payload: %.*s\n", pdu->payload_len, (char *)pdu->payload);
   }
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;

    // Define remote address
    sock_udp_ep_t remote = { .family = AF_INET6,
                             .port = SERVER_PORT };

    // Parse server IP
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, COAP_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Set payload
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));

    // Send CoAP request
    gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}