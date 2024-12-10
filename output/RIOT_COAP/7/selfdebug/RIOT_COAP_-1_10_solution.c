#include <stdio.h>
#include <string.h>
#include <net/gcoap.h>
#include <net/af.h> 
#include <net/sock/udp.h>

define CONFIG_GCOAP_PDU_BUF_SIZE (128)

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
   if (memo->state == GCOAP_MEMO_UNUSED) {
       printf("Response received: %u\n", coap_get_code(pdu));
   }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t result;

    /* Initialize the remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* Prepare a CoAP POST request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    /* Send the request */
    result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Error sending request: %d\n", (int)result);
    }
    
    return 0;
}