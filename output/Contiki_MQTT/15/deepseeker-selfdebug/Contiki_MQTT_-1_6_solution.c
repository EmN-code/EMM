
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/uip-udp-packet.h"
#include "net/ip/uip-icmp6.h"
#include "net/ip/uip-sr.h"
#include "net/ip/uip-packetqueue.h"
#include "net/ip/uip-split.h"
#include "net/ip/uip-fw.h"
#include "net/ip/uip-fw-drv.h"
#include "net/ip/uip-fw-drv-nrf52840.h"
#include "net/ip/uip-fw-drv-nrf52840-ble.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-adv.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-scan.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-conn.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-client.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-service.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-descriptor.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-write.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-read.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-notify.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-register.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with-reason.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with-reason-and-callback.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with-reason-and-callback-and-user_data.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with-reason-and-callback_and_user_data_and_user_data_size.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf52840-ble-gatt-server-characteristic-value-indicate-confirm-callback-unregister-all-with_reason_and_callback_and_user_data_and_user_data_size_and_user_data_size_max_and_user_data_size_max_max_and_user_data_size_max_max_max_and_user_data_size_max_max_max_max_and_user_data_size_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_max_max_and_user_data_size_max_max_max_max_max_max_max_max_max_max_max_max_max_max.h"
#include "net/ip/uip-fw-drv-nrf5284