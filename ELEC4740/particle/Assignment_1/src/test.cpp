/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#include <cstring>
#include <stdio.h>
// #include <math.h>
// We define MY_LED to be the LED that we want to blink.
//
// In this tutorial, we're using the blue D7 LED (next to D7 on the Photon
// and Electron, and next to the USB connector on the Argon and Boron).
void setup();
void loop();


int ledPin1G = D3;
int ledPin1R = D2;
int ledPin2G = D7;
int ledPin2R = D8;
int ledPinW = D6;
int buttonPin = D5;
SerialLogHandler logHandler;
enum LED1 {G1,R1};
LED1 led1 = G1;
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
int timer = 200;
uint8_t check = 0xcc;  //1-3 (header), (4-9) (data), (10)
size_t offset;
// int add_1 = *lux_avg; //float
// int add_2 = *movement;
// uint8_t out_buffer[6];
// Timer timer(10, white_led_timer);

// int movement_det = 0;

// The following line is optional, but recommended in most firmware. It
// allows your code to run before the cloud is connected. In this case,
// it will begin blinking almost immediately instead of waiting until
// breathing cyan,
SYSTEM_THREAD(ENABLED);

// The setup() method is called once when the device boots.

// void white_led_timer(){
// void setAdvertisingData(float,uint16_t);
void float2Bytes(float,byte*);
// 	timer.changePeriodFromISR(newPeriod);
// }

void setup()
{
	// In order to set a pin, you must tell Device OS that the pin is
	// an OUTPUT pin. This is often done from setup() since you only need
	// to do it once.
	(void)logHandler; // Does nothing, just to eliminate the unused variable warning

	BLE.on();
	// pinMode(MY_LED, OUTPUT);
	pinMode(ledPin1G, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(ledPin1R, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(ledPin2G, OUTPUT_OPEN_DRAIN_PULLUP);
	pinMode(ledPin2R, OUTPUT_OPEN_DRAIN_PULLUP);
	// pinMode(ledPin2, OUTPUT);
	pinMode(ledPinW, OUTPUT);
	pinMode(buttonPin, INPUT_PULLDOWN);

	
}

// The loop() method is called frequently.
void loop()
{
	a = !a;
	// POTENTIOMETER AND WHITE LED
	potentiometer = analogRead(analogPin4);
	if (potentiometer < 200){
		duty_cycle = 0;
	} else{
		duty_cycle = (potentiometer-200)/15.278;
	}
	analogWrite(ledPinW,duty_cycle);

	// READING
	movement = analogRead(analogPin);
	adc_lux = analogRead(analogPin2);

	float lux = -206.2*adc_lux/4096*3.3+473.43;
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
	//timer 60s
	timer = 5*60; 
	} else if (movement > 2000) {
			led2 = R2;
	//timer 60s
	timer = 5*60;
	} else if (timer <=0) {
		led2 = OFF;
	}

	// BUTTON LED OPERATION
	buttonPin = digitalRead(D5);
	// Log.info("%d", debounce);
	if (debounce <=  0) {
		if (buttonPin == 1) {
			if (led1 == G1) {
				led1 = R1;
			} else {
				led1 = G1;
			}
			debounce = 10;
		}
	} else {
		debounce--;  
	}
	// Log.info("Lux_Avg: {%f}, Mov_Sen: {%d}", lux_avg, movement);
	
	uint8_t buf[BLE_MAX_ADV_DATA_LEN];
	offset = 0;

	// Manufacturer-specific data
	// 16-bit: Company ID (0xffff)
	// Byte: Internal packet identifier (0x55)
	// 32-bit: Color code

	// Company ID (0xffff internal use/testing)
	buf[offset++] = 0xab;
	buf[offset++] = 0xcd;
	// Internal packet type. This is arbitrary, but provides an extra
	// check to make sure the data is my data, since we use the 0xffff company
	// code.
	buf[offset++] = 0x33;
	Log.info("lux = {%f}",lux_avg);
	Log.info("movement = {%d}",movement);
	// Our specific data, color code
	// float* pf = &lux_avg;
	memcpy(&buf[offset], &lux_avg, 4);
	// Log.info("pf = {%f}",pf);
	offset += 4;
	memcpy(&buf[offset], &movement, 2); 
	offset += 2;
	memcpy(&buf[offset], &check, 1); 
	Log.info("check = %x",check);
	offset += 1;

	// timer.start();
	BleAdvertisingData advData;
	advData.appendCustomData(buf, offset);

	// Advertise every 100 milliseconds. Unit is 0.625 millisecond intervals.
	BLE.setAdvertisingInterval(250);

	// Continuously advertise
	BLE.advertise(&advData);

	
	
	switch(led1){
	case G1:
		// flashing green
		digitalWrite(ledPin1G,a);
		digitalWrite(ledPin1R,1);
		break;
	case R1:
		// flashing red
		digitalWrite(ledPin1G,1);
		digitalWrite(ledPin1R,a);
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
  	// analogWrite(ledPin, val/16);
	// if (val == 0){
	// 	digitalWrite(ledPin1G,1);
	// } else{
	// 	digitalWrite(ledPin1G,0);
	// }
	// Log.info("Green = %d, Red = %d",digitalRead(ledPin2G), digitalRead(ledPin2R));
	delay(200);
	// And repeat! 
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