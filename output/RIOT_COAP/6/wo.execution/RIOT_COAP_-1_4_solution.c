#include "thread_config.h"
#include "thread.h"
#include "sys/socket.h"
#include "sock/udp.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include <stdint.h>
#include <stdio.h>

int main() {
    // Configuration for the destination CoAP server
    const char *server_ip = "47.102.103.1";
    uint16_t server_port = 5683;
    const char *uri_path = "/coap/test";
    
    // Message to send
    const char *msg = "Hello, COAP Cloud";
    
    // Buffer for the CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Remote address configuration
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = server_port;
    inet_pton(AF_INET, server_ip, &remote.addr.ipv4);

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, uri_path);

    // Add payload to the request
    memcpy(pdu.payload, msg, strlen(msg));
    pdu.payload_len = strlen(msg);

    // Send the CoAP request
    ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (result <= 0) {
        printf("Error sending request\n");
    } else {
        printf("Request sent successfully\n");
    }

    return 0;
}
