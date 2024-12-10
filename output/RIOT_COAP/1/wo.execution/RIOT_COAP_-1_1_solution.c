#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include <stdio.h>
#include <stdlib.h>

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define COAP_METHOD COAP_METHOD_POST

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received from CoAP server\n");
}

int main(void) {
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };

    // Convert the IP address
    if (inet_pton(AF_INET, COAP_SERVER_ADDR, &remote.addr) != 1) {
        puts("ERROR: unable to encode destination address\n");
        return 1;
    }

    // Initialize the CoAP request
    ssize_t len = gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD, URI_PATH);
    if (len < 0) {
        puts("ERROR: unable to initialize CoAP request\n");
        return 1;
    }

    // Set the payload
    size_t payload_len = strlen(PAYLOAD);
    memcpy(pdu.payload, PAYLOAD, payload_len);
    gcoap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    pdu.payload_len = payload_len;

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload_len + len, &remote, _resp_handler, NULL);

    // Main loop
    while (1) {
        // Simulating long-running process that repeatedly sends requests
        ztimer_sleep(ZTIMER_USEC, 10 * 1000 * 1000); // Sleep for 10 seconds
        gcoap_req_send(buf, pdu.payload_len + len, &remote, _resp_handler, NULL);
    }

    return 0;
}