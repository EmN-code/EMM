#include <thread.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "esp_wifi.h"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* handle the response */
    return 0;
}

int main(void) {
    /* Setup CoAP client */
    static gcoap_listener_t listener = {
        .resp_handler = _resp_handler
    };
    gcoap_register_listener(&listener);

    /* Initialize networking */
    esp_wifi_setup();

    /* Define remote server endpoint */
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv6);

    /* Prepare CoAP request */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    coap_pkt_append_payload_marker(&pdu);
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    /* Send the CoAP request */
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request: %d\n", res);
    }

    return 0;
}