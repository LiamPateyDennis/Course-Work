#include "Particle.h"

// This example does not require the cloud so you can run it in manual mode or
// normal cloud-connected mode
// SYSTEM_MODE(MANUAL);

// These UUIDs were defined by Nordic Semiconductor and are now the defacto standard for
// UART-like services over BLE. Many apps support the UUIDs now, like the Adafruit Bluefruit app.
// Severice UUID
SerialLogHandler logHandler; 
const BleUuid serviceUuid("bfc37fc6-64b1-4f2d-a3e5-c0df90bd5cdd");
// Char UUID
const BleUuid RxSN2Uuid("e503d4f7-ad9a-4131-a192-7150b3a7fc78");
// const BleUuid SoundUuid("06ddddf7-e00d-4ee8-83eb-bb7317c8589b");
const BleUuid TxSN2Uuid("11ba35cc-8edb-49a0-ac68-375dee5f77ff");
const size_t RxSN2_BUF_SIZE = 20;
// const size_t SoundUuid_BUF_SIZE = 20;
// const size_t ButtonUuid_BUF_SIZE = 20;
const size_t SCAN_RESULT_COUNT = 20;
BleScanResult scanResults[SCAN_RESULT_COUNT];

BleCharacteristic Central_Tx("tx_central",BleCharacteristicProperty::WRITE,TxSN2Uuid,TxSN2Uuid);
// BleCharacteristic Central_Tx("tx_central",BleCharacteristicProperty::NOTIFY,TxSN2Uuid,TxSN2Uuid);

BleCharacteristic peerTempUuid;
BleCharacteristic peerSoundUuid;
BleCharacteristic peerButtonUuid;
BlePeerDevice peer;

uint8_t txBuf[RxSN2_BUF_SIZE];
size_t txLen = 0;

float num;
uint16_t sound;
uint16_t button_var;
uint16_t count;

const unsigned long SCAN_PERIOD_MS = 2000;
unsigned long lastScan = 0;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    // for (size_t ii = 0; ii < len; ii++) {
    //     Serial.write(data[ii]);
    // }
	memcpy(&num,&data[0],4);
	memcpy(&sound,&data[4],2);
	memcpy(&button_var,&data[6],2);

	// Log.info("\nfloat = %f\n",num); 
	// Log.info("\nfloat = %d\n",sound); 
	// Log.info("\nfloat = %d\n",button_var); 

	// memcpy(&count,&data,2);
	// print


}

// void onDataReceived2(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
//     // for (size_t ii = 0; ii < len; ii++) {
//     //     Serial.write(data[ii]);
//     // }
// 	memcpy(&num,&data[0],4);
// 	memcpy(&sound,&data[4],2);
// 	memcpy(&button_var,&data[6],2);

// 	Log.info("\nfloat = %f\n",num); 
// 	Log.info("\nfloat = %d\n",sound); 
// 	Log.info("\nfloat = %d\n",button_var); 
// }

void setup() {
    Serial.begin();
	BLE.on();
    peerTempUuid.onDataReceived(onDataReceived, &peerTempUuid);
	// peerTempUuid.onDataReceived(onDataReceived2, &peerTempUuid);
	// peerTempUuid.onDataReceived(onDataReceived, &peerSoundUuid);
	// peerTempUuid.onDataReceived(onDataReceived, &peerButtonUuid);
	BleAdvertisingData data;
	data.appendServiceUUID(serviceUuid);
	BLE.advertise(&data);
}

void loop() {
    if (BLE.connected()) {
        // while (Serial.available() && txLen < RxSN2_BUF_SIZE) {
        //     // txBuf[txLen++] = Serial.read();
		// 	memcpy(&txBuf[0],&num,4);
		// 	txLen+=4;
		// 	memcpy(&txBuf[4],&sound,2);
		// 	txLen+=2;
		// 	memcpy(&txBuf[6],&button_var,2);
		// 	txLen+=2;
        //     // Serial.write(txBuf[txLen - 1]);
        // }
        // if (txLen > 0) {
        // 	// Transmit the data to the BLE peripheral
        //     
        //     txLen = 0;
        // }
		count++;
		memcpy(&txBuf[0],&count,2);
		Central_Tx.setValue(txBuf, 2);
		Log.info("Count: %d",count);
		delay(1000);
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
							peer.getCharacteristicByUUID(peerTempUuid, RxSN2Uuid);
							// peer.getCharacteristicByUUID(peerSoundUuid, SoundUuid);
							// peer.ge
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
