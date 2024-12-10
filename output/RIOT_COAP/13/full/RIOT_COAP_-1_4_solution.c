#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ipv6/addr.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "esp_system.h"
#include "time_units.h"

static const char *MQTT_BROKER_IP = "47.102.103.1";
static const uint16_t MQTT_BROKER_PORT = 5683;
static const char *EMQX_URI_PATH = "/coap/test";

static ssize_t _resp_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len) {
    (void)pdu;
    (void)buf;
    (void)len;
    printf("Response received!\n");
    return len;
}

void send_coap_request(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, MQTT_BROKER_IP);
    remote.port = MQTT_BROKER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int retry_count = 0;
    int rc;

    while (1) {
        gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
        coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);  
        memcpy(pdu.payload, "Hello, COAP Cloud", 17);
        pdu.payload_len = 17;

        rc = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("CoAP request error. Retry %d\n", ++retry_count);
            ztimer_sleep(ZTIMER_USEC, 1000000);
            continue;
        } else {
            printf("CoAP request sent successfully.\n");
            break;
        }
    }
}

int main(void) {
    printf("Starting CoAP client example\n");
    send_coap_request();
    return 0;
}