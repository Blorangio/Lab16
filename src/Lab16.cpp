/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/Documents/CTD_2023/Lab16/src/Lab16.ino"
#include "oled-wing-adafruit.h"
void setup();
void loop();
#line 2 "/Users/admin/Documents/CTD_2023/Lab16/src/Lab16.ino"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

const size_t UART_TX_BUF_SIZE = 20;

OledWingAdafruit display;

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");

const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);

uint8_t txBuf[UART_TX_BUF_SIZE];
String message = "Argon Connected!";

BleAdvertisingData data;

LEDStatus solidYellow(RGB_COLOR_YELLOW, LED_PATTERN_SOLID, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);
LEDStatus solidBlue(RGB_COLOR_BLUE, LED_PATTERN_SOLID, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

bool firstConnection = true;
int indexer = 0;
void setup()
{
  display.loop();
  Serial.begin(9600);
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);
  BLE.setDeviceName("Fenton");

  display.setup();
	display.clearDisplay();
	display.display();
}

void loop()
{
  if (BLE.connected())
  {
    solidBlue.setActive(true);
    if (firstConnection)
    {
      message.toCharArray((char *)txBuf, message.length() + 1);
      txCharacteristic.setValue(txBuf, message.length() + 1);
      indexer++;
      if(indexer>20) {
        firstConnection = false;
      }
    }
  }
  else
  {
    indexer = 0;
    solidYellow.setActive(true);
  }
  delay(1000);
}

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
  if (data[0] == 49 && len)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.println("you hit a 1");
    display.display();
  }
  else if (data[0] == 48 && len)
  {
    BLE.disconnect();
  }
  Serial.println(data[0]);
}