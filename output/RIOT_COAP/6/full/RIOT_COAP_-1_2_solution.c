#include <sys/socket.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define RETRY_INTERVAL (1U * US_PER_SEC) // 1 second interval for retrying

static ssize_t _resp_handler(coap_pkt_t *pdu, void *context) {
    (void)context;
    printf("Response received, payload length: %d", coap_get_payload_len(pdu));
    return 0;
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t res;
    
    // Initialize the remote address
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    sock_str2ipv6(SERVER_IP, (ipv6_addr_t *)&remote.addr.ipv6); // Ensure using correct IP address format

    // Initialize the CoAP PDU for POST request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    
    // Append the message to the PDU
    coap_payload(&pdu, (uint8_t *)COAP_MESSAGE, sizeof(COAP_MESSAGE) - 1);

    // Send CoAP request with retry logic
    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Failed to send CoAP request, retrying in 1 second\n");
            ztimer_sleep(ZTIMER_USEC, RETRY_INTERVAL);
            continue;
        } else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}