#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "net/emcute.h"

static ssize_t _resp_handler(coap_pkt_t *pdu, const sock_udp_ep_t *remote, void *context) {
    (void)context;   // Context is unused
    
    if (pdu->payload_len > 0) {
        printf("Response payload: %s\n", (char *)pdu->payload);
    }
    else {
        printf("No response payload\n");
    }
    
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;
    sock_udp_ep_t remote;

    // Define remote. This is the CoAP server address
    remote.family = AF_INET;
    remote.port = 5683;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    coap_hdr_set_code(pdu.hdr, COAP_METHOD_POST);

    // Add payload
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    coap_hdr_set_payload_len(pdu.hdr, strlen(payload));  // Correct function for payload length

    // Send the CoAP request
    bytes = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes <= 0) {
        printf("Error sending request\n");
    }
    else {
        printf("Sent %d bytes\n", (int)bytes);
    }

    return 0;
}