/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "app.h"

#include "gatt_db.h"
#include "Timer.h"

#define data_size 242
int16_t set_min = -32768;
int16_t set_max = -32768;
//int16_t set_min = 0;
//int16_t set_max = 0;
static bool nflag = false;
uint8_t data[data_size]={0};
static sl_status_t send_packet(void);
static void transfer_data(void);
void increment_data(void);


// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{

  sl_bt_system_halt(1);
  //sl_bt_system_set_tx_power(-32768,-32768,&set_min,&set_max);
  sl_bt_system_set_tx_power(0,0,&set_min,&set_max);
  sl_bt_system_halt(0);
  Timer_SetupAll();
  //Timer_Start();
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  if(nflag){
      increment_data();
      transfer_data();
  }
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

void set_notify_flag(){
  nflag = true;
}

void LETIMER0_IRQHandler(void){
  LETIMER_IntClear(LETIMER0, LETIMER_IF_UF);
  set_notify_flag();
}


/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        1000, // min. adv. interval (milliseconds * 1.6)
        1000, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);
      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);
      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Restart advertising after client has disconnected.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);
      break;

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    case sl_bt_evt_gatt_server_attribute_value_id:
         // The value of the gattdb_datareq characteristic was changed.
         if (gattdb_datareq == evt->data.evt_gatt_server_characteristic_status.characteristic) {
           uint8_t data_recv;
           size_t data_recv_len;

           // Read characteristic value.
           sc = sl_bt_gatt_server_read_attribute_value(gattdb_datareq,
                                                       0,
                                                       sizeof(data_recv),
                                                       &data_recv_len,
                                                       &data_recv);
           // Initiate Data Transfer.
           if (data_recv == 0x01) {
               //printf("Initiating data transfer\n");
               Timer_Start();
           } else if (data_recv == 0x00) {
               Timer_Stop();
             //printf("Null data request recieved.\n");
           } else {
               float i=0;
             switch(data_recv)
             {
             case 0x02: i=0.400;break;
             case 0x03: i=0.300;break;
             case 0x04: i=0.200;break;
             case 0x05: i=0.100;break;
             case 0x06: i=0.090;break;
             case 0x07: i=0.080;break;
             case 0x08: i=0.070;break;
             case 0x09: i=0.060;break;
             case 0x0A: i=0.050;break;
             case 0x0B: i=0.045;break;
             case 0x0C: i=0.040;break;
             case 0x0D: i=0.035;break;
             case 0x0E: i=0.030;break;
             case 0x0F: i=0.025;break;
             case 0x10: i=0.020;break;
             case 0x11: i=0.015;break;
             case 0x12: i=0.010;break;
             case 0x13: i=0.009;break;
             case 0x14: i=0.008;break;
             case 0x15: i=0.007;break;
             case 0x16: i=0.006;break;
             case 0x17: i=0.005;break;
             case 0x18: i=0.004;break;
             case 0x19: i=0.003;break;
             case 0x1A: i=0.002;break;
             default: break;
             }
             if (i!=0){
                 Timer_Set_Interval(i);
             }
             //printf("Invalid datareq value: 0x%02x\n", (int)data_recv);
           }
         }
         break;

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}

//Increments data array for testing
void increment_data(void){
  uint8_t carry = 1; // Initialize carry to 1 to start the increment
  for (size_t i = sizeof(data); i > 0; i--) {
      uint16_t sum = data[i - 1] + carry; // Add current byte and carry
      data[i - 1] = (uint8_t)sum; // Update the byte with the lower 8 bits of the sum
      carry = sum >> 8; // Update carry with the upper 8 bits of the sum
      if (carry == 0) {
          break; // If there's no carry left, we're done
      }
  }
}

//Performs data transfer with num packets = 1
static void transfer_data(void)
{
    sl_status_t sc;
    sc = send_packet();
    nflag=false;
}


//Sends a packet of data
static sl_status_t send_packet(void)
{
  sl_status_t sc;

  sc = sl_bt_gatt_server_notify_all(gattdb_notifier,
                                    sizeof(data),
                                    &data);

  return sc;
}
