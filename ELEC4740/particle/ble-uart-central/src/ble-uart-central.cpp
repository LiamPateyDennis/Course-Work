#include "Particle.h"

// This example does not require the cloud so you can run it in manual mode or
// normal cloud-connected mode
// SYSTEM_MODE(MANUAL);
SerialLogHandler LogHandler;
// These UUIDs were defined by Nordic Semiconductor and are now the defacto standard for
// UART-like services over BLE. Many apps support the UUIDs now, like the Adafruit Bluefruit app.
const BleUuid serviceUuid("7828f55d-c04c-49e8-86e9-8f1362755dc7");
const BleUuid rxUuid("75c14d44-1b11-4514-b221-df15fc88050f");
const BleUuid txUuid("10fcd87a-8601-484e-829d-d114a40a8ba6");

const size_t UART_TX_BUF_SIZE = 20;
const size_t SCAN_RESULT_COUNT = 20;

BleScanResult scanResults[SCAN_RESULT_COUNT];

BleCharacteristic peerTxCharacteristic;
BleCharacteristic peerRxCharacteristic;
BlePeerDevice peer;


uint8_t txBuf[UART_TX_BUF_SIZE];
size_t txLen = 0;

uint16_t counting;

const unsigned long SCAN_PERIOD_MS = 2000;
unsigned long lastScan = 0;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    // for (size_t ii = 0; ii < len; ii++) {
    //     Serial.write(data[ii]);
    // }
	memcpy(&counting,&data[0],2);
	Log.info("%d",counting);
}

void setup() {
    Serial.begin();
	BLE.on();

#if SYSTEM_VERSION == SYSTEM_VERSION_v310 
	// This is required with 3.1.0 only
	BLE.setScanPhy(BlePhy::BLE_PHYS_AUTO);
#endif

    peerTxCharacteristic.onDataReceived(onDataReceived, &peerTxCharacteristic);
}

void loop() {
    if (BLE.connected()) {
		uint8_t txBuf[UART_TX_BUF_SIZE];
		size_t txLen = 0;
		counting++;
		counting++;
		memcpy(&txBuf[0], &counting,2);
        peerRxCharacteristic.setValue(txBuf,2);
		// Log.info("Connection");

    }
    else {
    	if (millis() - lastScan >= SCAN_PERIOD_MS) {
    		// Time to scan
    		lastScan = millis();

    		int count = BLE.scan(scanResults, SCAN_RESULT_COUNT);
			if (count > 0) {
				for (uint8_t ii = 0; ii < count; ii++) {
					// Our serial peripheral only supports one service, so we only look for one here.
					// In some cases, you may want to get all of the service UUIDs and scan the list
					// looking to see if the serviceUuid is anywhere in the list.
					BleUuid foundServiceUuid;
					size_t svcCount = scanResults[ii].advertisingData().serviceUUID(&foundServiceUuid, 1);
					if (svcCount > 0 && foundServiceUuid == serviceUuid) {
						peer = BLE.connect(scanResults[ii].address());
						if (peer.connected()) {
							peer.getCharacteristicByUUID(peerTxCharacteristic, txUuid);
							peer.getCharacteristicByUUID(peerRxCharacteristic, rxUuid);

							// Could do this instead, but since the names are not as standardized, UUIDs are better
							// peer.getCharacteristicByDescription(peerTxCharacteristic, "tx");
						}
						break;
					}
				}
			}
    	}

    }
}
