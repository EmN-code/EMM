#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "string.h"
#include "stdout.h"
#include "stdio.h"
#include "netinet/in.h"
#include "netinet/udp.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/conn/ip.h"
#include "net/af.h"
#include "kernel_defines.h"
#include "ipv6_addr_from_str.h"
#include "ip_addr.h"
#include "in.h"
#include "inet.h"
#include "gcoap.h"
#include "gcoap_msg.h"
#include "gcoap_handlers.h"
#include "gcoap_resource.h"
#include "gcoap_types.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "coap_hdr_set_type.h"
#include "coap_hdr_get_type.h"
#include "coap_option.h"
#include "coap_pkt.h"
#include "coap_types.h"

int main(void){
    // Initialization block
    printf("Starting CoAP client example\n");
    char *message = "Hello, COAP Cloud";
    const char *server_uri_path = "/coap/test";
    const char *server_ip = "47.102.103.1";
    sock_udp_ep_t server = { .family = AF_INET, .port = 5683 };
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;

    if(inet_pton(AF_INET, server_ip, &server.addr.ipv4) != 1){
        printf("Failed to convert server IP\n");
        return 1;
    }

    while (1) {
        // Initialize CoAP request
        len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, server_uri_path);
        if (len == 0) {
            printf("Failed to initialize request\n");
        }

        // Copy the payload
        memcpy(pdu.payload, message, strlen(message));
        gcoap_finish(&pdu, len + strlen(message));

        // Attempt to send the request
        if(gcoap_req_send(buf, pdu.payload_len, &server, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0 ) {
            printf("Failed to send CoAP POST request\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            printf("CoAP POST request sent successfully\n");
            break;
        }
    }

    return 0;
}