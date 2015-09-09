/*
This sketch demonstrates how to coordinate data
between 3 devices in a Gazell network.

The host collects RSSI samples from the Devices,
and determines which device has the strongest
average RSSI (ie: the Device that is closest
to the Host).  The Green led is set on the
closest Device.

Since the Device must initiate communication, the
device "polls" the Host evey 200ms.
*/

#include <RFduinoGZLL.h>
#include <RFduinoBLE.h>

device_t role = DEVICE0;

// pin for the Green Led
int green_led = 3;

void setup()
{
  pinMode(green_led, OUTPUT);

    // the host/device base address can be changed to create independent networks in the same area
  // (note: the msb cannot be 0x55 or 0xaa)
  RFduinoGZLL.hostBaseAddress = 0x12345678;    // default host base address is 0x0D0A0704;
  RFduinoGZLL.deviceBaseAddress = 0x87654321;  // default device base address is 0x0E0B0805;
  

  RFduinoGZLL.txPowerLevel = 4;

}

void loop()
{
  RFduinoBLE.end();
  delay(100);

  NRF_RADIO->TASKS_RSSISTOP = 0;
  RFduinoGZLL.begin(role); // consumes a few mA
  RFduinoGZLL.hostBaseAddress = 0x12345678;    // default host base address is 0x0D0A0704;
  RFduinoGZLL.deviceBaseAddress = 0x87654321;  // default device base address is 0x0E0B0805;
  delay(150);
  RFduinoGZLL.sendToHost("hmm");
  delay(100);
  RFduinoGZLL.end();
  NRF_RADIO->TASKS_RSSISTOP = 1; // add this here

  RFduinoBLE.begin();
  delay(1000);
  RFduino_ULPDelay(4000);

  /*
  //delay(200);
  RFduinoGZLL.end();

  RFduinoBLE.begin();
  RFduino_ULPDelay(SECONDS(3.0));
  RFduinoBLE.end();

  RFduinoGZLL.begin(role);

  // request the state from the Host (send a 0 byte payload)
  RFduinoGZLL.sendToHost(NULL, 0);

  delay(200);

  RFduinoGZLL.end();
  */
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  
  // ignore acknowledgement without payload
  if (len > 0)
  {
    // set the Green led if this device is the closest device
    device_t closest_device = (device_t)data[0];
    digitalWrite(green_led, (role == closest_device));
    Serial.println(data[0]);
  }
  
}
