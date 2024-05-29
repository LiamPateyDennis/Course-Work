#include "Particle.h"
#include "LiquidCrystal_I2C_Spark.h"
#include <iostream>
#include <string>



LiquidCrystal_I2C *lcd;

// This example does not require the cloud so you can run it in manual mode or
// normal cloud-connected mode
// SYSTEM_MODE(MANUAL);
const BleUuid serviceUuid("7828f55d-c04c-49e8-86e9-8f1362755dc7");
const BleUuid rxUuid("75c14d44-1b11-4514-b221-df15fc88050f");
const BleUuid txUuid("10fcd87a-8601-484e-829d-d114a40a8ba6");

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

const size_t UART_TX_BUF_SIZE = 20;
const size_t SCAN_RESULT_COUNT = 20;

BleScanResult scanResults[SCAN_RESULT_COUNT];

BleCharacteristic peerTxCharacteristic;
BleCharacteristic peerRxCharacteristic;
BlePeerDevice peer;

class MyTimer {
public:
    MyTimer(unsigned long intervalMs, void (*callback)()) : intervalMs_(intervalMs), callback_(callback) {
        lastTime_ = millis();
    }

    void update() {
        unsigned long currentTime = millis();
        if (currentTime - lastTime_ >= intervalMs_) {
            lastTime_ = currentTime;
            callback_();
        }
    }

private:
    unsigned long intervalMs_;
    unsigned long lastTime_;
    void (*callback_)();
};

// HOW TO USE CLASS

// int count = 0;

// void print_every_second() {
//     Serial.printf("count=%d\n", count++);
// }

// void setup() {
//     Serial.begin(9600);
//     MyTimer myTimer(1000, print_every_second);
// }

// void loop() {
//     // Other loop logic here
//     // ...

//     // Call the timer update method
//     myTimer.update();
// }

// MORE BLUETOOTH VARIABLES
uint8_t txBuf[UART_TX_BUF_SIZE];
size_t txLen = 0;

uint16_t counting;
uint8_t duty_cycle_overide;

const unsigned long SCAN_PERIOD_MS = 2000;
unsigned long lastScan = 0;


const size_t SCAN_RESULT_MAX = 30;
const pin_t blue_led = D7;
int BOOL_var = 0;
bool refresh_lcd = false;
bool flash_led = false;

// int lastSecond = 0;
char* buffer;

uint8_t button_SN1;
uint8_t button_SN2;

float lux;
float temp;
uint16_t movement;
uint16_t sound;
uint8_t offset;
uint8_t debounce;
uint8_t EMERGENCY_NOTIFY = 0;
uint8_t return_button = 0;


uint8_t tick[8] = {
  0b00001,
  0b00001,
  0b00011,
  0b00010,
  0b10110,
  0b11100,
  0b01000,
  0b00000
};

uint8_t cross[8] = {
  0b00000,
  0b00000,
  0b10001,
  0b11011,
  0b01110,
  0b11011,
  0b10001,
  0b00000
};

int LED1G = D3;
int LED1R = D2;
int LED2G = D5;
int LED2R = D4;
int LED3G = D7;
int LED3R = D6;
int BUTTON = D8;

enum LED {G,R,FG,FR,OFF};
LED led1 = OFF;
LED led2 = OFF;
LED led3 = OFF;

void loop_2s() {
	refresh_lcd = true;
}

void led_flashing() {
	flash_led = true;
	// Implement LED FLASHING
}

MyTimer timer(2000,loop_2s);
MyTimer timer2(250,led_flashing);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    // for (size_t ii = 0; ii < len; ii++) {
    //     Serial.write(data[ii]);
    // }
	memcpy(&lux,&data[0],4);
	memcpy(&movement,&data[4],2);
	memcpy(&EMERGENCY_NOTIFY,&data[6],1);
	// Log.info("movement = %d",movement);
	Log.info("%d",EMERGENCY_NOTIFY);
}



// BleScanResult scanResults[SCAN_RESULT_MAX];
LEDStatus ledOverride(RGB_COLOR_WHITE, LED_PATTERN_SOLID, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);




void setup() {

	Serial.begin(9600);
	BLE.on();

	lcd = new LiquidCrystal_I2C(0x27, 16, 2);
	lcd->init();
	lcd->backlight();
	lcd->clear();

	lcd->load_custom_character(1, tick);
	lcd->load_custom_character(0, cross);

	pinMode(BUTTON, INPUT_PULLDOWN);
	pinMode(LED1G, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(LED1R, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(LED2G, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(LED2R, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(LED3G, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(LED3R, OUTPUT_OPEN_DRAIN_PULLUP);

#if SYSTEM_VERSION == SYSTEM_VERSION_v310
	// This is required with 3.1.0 only
	BLE.setScanPhy(BlePhy::BLE_PHYS_AUTO);
#endif

	peerTxCharacteristic.onDataReceived(onDataReceived, &peerTxCharacteristic);
}

void loop() {
	// **** PUBLISH DATA TO CLOUD **** (missing)

	// BLUETOOTH TRANSMITTING
	if (BLE.connected()) {
		uint8_t txBuf[UART_TX_BUF_SIZE];
		size_t txLen = 0;
		// txBuf[0] = CANCEL_EMERGENCY_NOTIFY;
		memcpy(&txBuf,&return_button,1);
		memcpy(&txBuf[1],&duty_cycle_overide,1);
		txLen+=1;
		
		// CHECK
		counting++;
		if (counting >= 3000) {
			Log.info("Bluetooth Working");
			counting = 0;
		}
		led1 = G;
        peerRxCharacteristic.setValue(txBuf,2);

		if (return_button == 1){
			Log.info("Sending Return Button");
			return_button = 0;
		} 
    }

	// BLUETOOTH SCANNING
    else { 
		led1 = OFF;
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

	// LOGIC FUNCTIONS
	// if(button_SN1 == 1){
	// 	led1 = FR;
	// 	EMERGENCY_NOTIFY = 0;
	// }

	// BUTTON LED OPERATION
	BUTTON = digitalRead(D8);
	// Log.info("%d", debounce);
	if (debounce <=  0) {
		if (BUTTON == 1) {
			return_button = 1;
			debounce = 100;
		}
	} else {
		debounce--;  
	}
	
	if (led1 != OFF){
		if (EMERGENCY_NOTIFY == 1) {
			led1 = FR;
		} else {
			led1 = G;
		}
	}

	// CONTROL FUNCTIONS
	if (lux <= 20){
		duty_cycle_overide = 100-lux;
	} else {
		duty_cycle_overide = 0;
	}
	
	// LCD Screen Display
	if (refresh_lcd == true){
		char buffer[16];
		// lcd->print("")
		lcd->setCursor(0,0);
		snprintf(buffer, sizeof(buffer), "Movement:");
		lcd->print(buffer);
		lcd->setCursor(10,0);
		if (movement >= 2000) {
			lcd->write(1);
		} else {
			lcd->write(0);
		}
		// Print to display
		lcd->setCursor(0,1);
		// char buffer[16]; // Adjust buffer size as needed
		if (lux > 0) {
		snprintf(buffer, sizeof(buffer), "Lux:%.2f", lux);
		} else {
		snprintf(buffer, sizeof(buffer), "Lux: %.2f", 0.00);	
		}
		lcd->print(buffer);
		// delay(100);
		// Log.info("%d, %f\n", movement, float_num);
		refresh_lcd = false;
	}

	// LED'S
	if (flash_led == true){
		BOOL_var = !BOOL_var;
		switch (led1){
			case FR:
			digitalWrite(LED1R,BOOL_var);
			digitalWrite(LED1G,1);
			break;
			case G:
			digitalWrite(LED1R,1);
			digitalWrite(LED1G,0);
			break;
		}
		switch (led2){
			case FR:
			digitalWrite(LED2R,BOOL_var);
			digitalWrite(LED2G,1);
			break;
			case G:
			digitalWrite(LED2R,1);
			digitalWrite(LED2G,0);
			break;
		}
		
		flash_led = false;
	}

	// Update MyTimer class
	timer.update();
	timer2.update();
} 

