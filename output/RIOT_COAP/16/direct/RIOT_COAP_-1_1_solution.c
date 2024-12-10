#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH    "/coap/test"
#define COAP_METHOD COAP_POST

static gcoap_req_t req;

static void send_coap_request(void) {
    uint8_t buf[128];
    size_t len;

    len = coap_build_request(req.pdu, &buf[0], sizeof(buf), COAP_METHOD, URI_PATH);
    
    if (len > 0) {
        gcoap_req_send2(&req, buf, len, SERVER_ADDR, SERVER_PORT);
    }

    printf("CoAP POST request sent to %s:%u\n", SERVER_ADDR, SERVER_PORT);
}

int main(void) {
    gcoap_init();

    printf("Sending CoAP POST request to CoAP server.\n");
    send_coap_request();

    return 0;
}