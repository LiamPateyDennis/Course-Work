/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#include <cstring>
#include <stdio.h>
#include <iostream>

// BLUETOOTH VARIABLES
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





void setup();
void loop();



int ledPin1G = D3;
int ledPin1R = D2;
int ledPin2G = D7;
int ledPin2R = D8;
int ledPinW = D6;
int buttonPin = D5;
enum LED1 {G1,R1,OFF1};
LED1 led1 = OFF1;
enum LED2 {G2,R2,OFF};
LED2 led2 = OFF;
// int ledPin = D1;                // LED connected to digital pin D1
// int analogPin = A0;             // potentiometer connected to analog pin A0
uint16_t movement = 0;
int potentiometer = 0;
int analogPin = A0;
int analogPin2 = A2;
int analogPin4 = A4;
int debounce = 0;
int a = 0;
int b = 1;
float A = 154.71;
float C = 1033.19;
int duty_cycle = 0;
int adc_lux = 0;
float lux_array[5];
float sum = 0;
float lux_avg;
int i = 0;
// const size_t SCAN_RESULT_MAX = 30;
uint8_t EMERGENCY_NOTIFY;
uint8_t check = 0xcc;  //1-3 (header), (4-9) (data), (10)
size_t offset;
bool flash_led = false;
uint8_t button_ACK = 0;
uint8_t return_button = 0;
uint8_t duty_cycle_overide = 0;
bool flag_60s = false;
void led_flashing();
void timer60s_callback();

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    // COMMANDS FROM CN
	memcpy(&return_button,&data[0],1);
	memcpy(&duty_cycle_overide,&data[1],1);
	if(return_button == 1){
		EMERGENCY_NOTIFY = 0;
		// led1 = G1;
		return_button = 0;
		Log.info("Return Button Pressed");		
	}

}

// BleScanResult scanResults[SCAN_RESULT_MAX];
LEDStatus ledOverride(RGB_COLOR_WHITE, LED_PATTERN_SOLID, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

Timer timer(250,led_flashing);
Timer timer60s(6000,timer60s_callback);

void led_flashing() {
	flash_led = true;
	// Implement LED FLASHING
	// Log.info("IN FUNCTION = test");
}

void timer60s_callback() {
	flag_60s = false;
}

// The following line is optional, but recommended in most firmware. It
// allows your code to  run before the cloud is connected. In this case,
// it will begin blinking almost immediately instead of waiting until
// breathing cyan,
SYSTEM_THREAD(ENABLED);

// The setup() method is called once when the device boots.

void float2Bytes(float,byte*);

void setup()
{
	// In order to set a pin, you must tell Device OS that the pin is
	// an OUTPUT pin. This is often done from setup() since you only need
	// to do it once.
	Serial.begin(9600);
	BLE.on(); 
	BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);

	BleAdvertisingData data;
    data.appendServiceUUID(serviceUuid);
    BLE.advertise(&data);

	rxCharacteristic.onDataReceived(onDataReceived, NULL);

	pinMode(ledPin1G, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(ledPin1R, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(ledPin2G, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(ledPin2R, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(ledPinW, OUTPUT);
	pinMode(buttonPin, INPUT_PULLDOWN);

	timer.start();
	Log.info("Assignment_1");
}

// The loop() method is called frequently.
void loop()
{
	// BLUETOOTH RECIEVE CODE
	if (BLE.connected()) {
    	uint8_t txBuf[UART_TX_BUF_SIZE];
    	size_t txLen = 0;

		//BLUETOOTH CHECK CODE
        counting++;
        if (counting >= 3000) {
			Log.info("Bluetooth Working");
			counting = 0;
		}
		led1 = G1;
		// SEND DATA
		memcpy(&txBuf,&lux_avg,4);
		txLen+=4;
		memcpy(&txBuf[txLen],&movement,2);
		txLen+=2;
		memcpy(&txBuf[txLen],&EMERGENCY_NOTIFY,1);
		txLen+=1;
		// if(EMERGENCY_NOTIFY == 1) {
		// 	Log.info("Button");
		// }
		// EMERGENCY_NOTIFY = 0;
    	while(Serial.available() && txLen < UART_TX_BUF_SIZE) {
            txBuf[txLen++] = Serial.read();
            Serial.write(txBuf[txLen - 1]);
        }
        if (txLen > 0) {
            txCharacteristic.setValue(txBuf, txLen);
        }

    } else {
		led1 = OFF1;
	}
	

	// POTENTIOMETER AND WHITE LED
	potentiometer = analogRead(analogPin4);
	if (potentiometer < 200){
		duty_cycle = 0;
	} else{
		duty_cycle = (potentiometer-200)/15.278;
	}
	if (duty_cycle_overide != 0){
		duty_cycle = duty_cycle_overide;
	}
	Log.info("Duty_Cycle = %d",duty_cycle);
	analogWrite(ledPinW,duty_cycle);

	// READING
	movement = analogRead(analogPin);
	adc_lux = analogRead(analogPin2);

	float lux = -206.2*adc_lux/4096*3.3+473.43-20;
	for (i = 0; i < 4; i++) {
		lux_array[i+1] = lux_array[i];
	}
	lux_array[0] = lux;
	sum = 0;
	for (i=0;i < 5;i++) {
		sum = sum + lux_array[i];
	}
	lux_avg = sum/5;
	// LED 2
	if ((lux_avg > 20)&(movement > 2000)){
			led2 = G2;
			if (flag_60s == 1){
				flag_60s = 1;
				timer60s.reset();
			} else {
				flag_60s = 1;
				timer60s.start();
			}
	} else if (movement > 2000) {
			led2 = R2;
			if (flag_60s == 1){
				flag_60s = 1;
				timer60s.reset();
			} else {
				flag_60s = 1;
				timer60s.start();
			}
	}
	if (flag_60s == 0) {
		led2 = OFF;
	}

	// BUTTON LED OPERATION
	buttonPin = digitalRead(D5);
	// Log.info("%d", debounce);
	if (debounce <=  0) {
		if (buttonPin == 1) {
			if(EMERGENCY_NOTIFY == 0){
				EMERGENCY_NOTIFY = 1;
			} else if(EMERGENCY_NOTIFY == 1) {
				EMERGENCY_NOTIFY = 0;
			}
			Log.info("Button Pressed");
			debounce = 25;
		}
	} else {
		debounce--;  
	}
	if (led1 != OFF1){
		if(EMERGENCY_NOTIFY == 1){
			led1 = R1;
		} else{
			led1 = G1; 
		}
	}
	// Log.info("%d",EMERGENCY_NOTIFY); 
	// Log.info("%d",debounce);

	// FLASH LED'S
	if(flash_led == true) {
		a = !a;
		switch(led1){
		case G1:
			// flashing green
			digitalWrite(ledPin1G,0);
			digitalWrite(ledPin1R,1);
			break;
		case R1:
			// flashing red
			digitalWrite(ledPin1G,1);
			digitalWrite(ledPin1R,a);
			break;
		case OFF1:
			digitalWrite(ledPin1G,1);
			digitalWrite(ledPin1R,1);
			break;
		};
		switch(led2){
		case G2:
			// flashing green
			digitalWrite(ledPin2G,a);
			digitalWrite(ledPin2R,1);
			break;
		case R2:
			// flashing red
			digitalWrite(ledPin2G,1);
			digitalWrite(ledPin2R,a);
			break;
		case OFF:
			digitalWrite(ledPin2G,1);
			digitalWrite(ledPin2R,1);
			break;
		};
		flash_led = false;
	}
} 

void float2Bytes(float val,byte* bytes_array){
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}