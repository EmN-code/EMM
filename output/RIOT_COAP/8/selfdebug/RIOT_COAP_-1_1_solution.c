#include <stdio.h> 
#include <net/conn/udp.h> 
#include <net/gcoap.h> 
#include <xtimer.h> 
#include <thread.h> 

#define COAP_SERVER_ADDRESS "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

static ssize_t _on_response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;  // Unused parameter
    (void)remote; // Unused parameter
    
    if (pdu == NULL) {
        printf("No response received.\n");
        return GCOAP_MEMO_RESP_TRUNC;
    }
    
    printf("Response received: \n");
    for (size_t i = 0; i < pdu->payload_len; i++) {
        printf("%c", pdu->payload[i]);
    }
    printf("\n");
    return (ssize_t)pdu->payload_len;
}

void send_coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_ADDRESS);
    remote.port = COAP_SERVER_PORT;

    ssize_t bytes = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (bytes < 0) {
        printf("Error initializing request\n");
        return;
    }

    const char *msg = "Hello, COAP Cloud";
    memcpy(pdu.payload, msg, strlen(msg));
    pdu.payload_len = strlen(msg);

    bytes = gcoap_req_send(buf, pdu.payload_len + (pdu.payload - buf), &remote, _on_response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        printf("Error sending request\n");
    } else {
        printf("Request sent\n");
    }
}

int main(void) {
    printf("Sending CoAP request...\n");
    send_coap_request();
    
    // Small delay if you want to see the result before program exits in simulators
    while(1) {
        xtimer_sleep(10);
    }
    return 0;
}