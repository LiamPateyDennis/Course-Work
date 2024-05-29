#include "Particle.h"

// This example does not require the cloud so you can run it in manual mode or
// normal cloud-connected mode
// SYSTEM_MODE(MANUAL);

SerialLogHandler LogHandler;

const size_t UART_TX_BUF_SIZE = 20;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

// These UUIDs were defined by Nordic Semiconductor and are now the defacto standard for
// UART-like services over BLE. Many apps support the UUIDs now, like the Adafruit Bluefruit app.
const BleUuid serviceUuid("7828f55d-c04c-49e8-86e9-8f1362755dc7");
const BleUuid rxUuid("75c14d44-1b11-4514-b221-df15fc88050f");
const BleUuid txUuid("10fcd87a-8601-484e-829d-d114a40a8ba6");

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

uint16_t counting;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    // Log.trace("Received data from: %02X:%02X:%02X:%02X:%02X:%02X:", peer.address()[0], peer.address()[1], peer.address()[2], peer.address()[3], peer.address()[4], peer.address()[5]);

    // for (size_t ii = 0; ii < len; ii++) {
    //     Serial.write(data[ii]);
    // }
    memcpy(&counting,&data[0],2);
    Log.info("%d",counting);
}

void setup() {
    Serial.begin(9600);

	BLE.on();

    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);

    BleAdvertisingData data;
    data.appendServiceUUID(serviceUuid);
    BLE.advertise(&data);

    rxCharacteristic.onDataReceived(onDataReceived, NULL);

}

void loop() {
    if (BLE.connected()) {
    	uint8_t txBuf[UART_TX_BUF_SIZE];
    	size_t txLen = 0;
        counting++;
        memcpy(&txBuf,&counting,2);
        txLen+=2;
    	while(Serial.available() && txLen < UART_TX_BUF_SIZE) {
            txBuf[txLen++] = Serial.read();
            Serial.write(txBuf[txLen - 1]);
        }
        if (txLen > 0) {
            // memcpy(&count,)
            txCharacteristic.setValue(txBuf, txLen);
        }
        // Log.info("Connection");

    }
}
 