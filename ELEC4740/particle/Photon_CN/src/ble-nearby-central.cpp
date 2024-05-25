#include "Particle.h"
#include "LiquidCrystal_I2C_Spark.h"
#include <iostream>
#include <string>

LiquidCrystal_I2C *lcd;

// This example does not require the cloud so you can run it in manual mode or
// normal cloud-connected mode
// SYSTEM_MODE(MANUAL);

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

const size_t SCAN_RESULT_MAX = 30;
const pin_t blue_led = D7;
int BOOL_var = 0;

int lastSecond = 0;
char* buffer;

BleScanResult scanResults[SCAN_RESULT_MAX];
LEDStatus ledOverride(RGB_COLOR_WHITE, LED_PATTERN_SOLID, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

void setup() {
	// (void)logHandler; // Does nothing, just to eliminate the unused variable warning
	BLE.on();

	Serial.begin(9600);
	lcd = new LiquidCrystal_I2C(0x27, 16, 2);
	lcd->init();
	lcd->backlight();
	lcd->clear();
	lcd->printstr("***Spark Time***");
	// lcd->setCursor(0,1);
	// lcd->print("why doesn't this work");

	

#if SYSTEM_VERSION == SYSTEM_VERSION_v310
	// This is required with 3.1.0 only
	BLE.setScanPhy(BlePhy::BLE_PHYS_AUTO);
#endif 
  
}

void loop() {
	// Only scan for 500 milliseconds

	BLE.setScanTimeout(100);
	int count = BLE.scan(scanResults, SCAN_RESULT_MAX);

	uint32_t curColorCode;
	// uint32_t colorCode;
	float float_num;
	uint16_t movement;
	uint8_t check;
	int curRssi = -999;

	for (int ii = 0; ii < count; ii++) {
		uint8_t buf[BLE_MAX_ADV_DATA_LEN];
		size_t len;

		// When getting a specific AD Type, the length returned does not include the length or AD Type so len will be one less
		// than what we put in the beacon code, because that includes the AD Type.
		len = scanResults[ii].advertisingData().get(BleAdvertisingDataType::MANUFACTURER_SPECIFIC_DATA, buf, BLE_MAX_ADV_DATA_LEN);
		
		if (len == 10) {
			// We have manufacturer-specific advertising data (0xff) and it's 7 bytes (without the AD type)

			// Byte: BLE_SIG_AD_TYPE_MANUFACTURER_SPECIFIC_DATA (0xff)
			// 16-bit: Company ID (0xffff)
			// Byte: Internal packet identifier (0x55)
			// 32-bit: Color code

			if (buf[0] == 0xab && buf[1] == 0xcd && buf[2] == 0x33) {
				// Company ID and internal packet identifier match
				memcpy(&check, &buf[9],1);
				if (check = 0xcc);
				memcpy(&float_num, &buf[3],4);
				memcpy(&movement, &buf[7], 2);
				

				// Log.info("colorCode: 0x%lx rssi=%d address=%02X:%02X:%02X:%02X:%02X:%02X ",
				// 		colorCode, scanResults[ii].rssi(),
				// 		scanResults[ii].address()[0], scanResults[ii].address()[1], scanResults[ii].address()[2],
				// 		scanResults[ii].address()[3], scanResults[ii].address()[4], scanResults[ii].address()[5]);

				// if (scanResults[ii].rssi() > curRssi) {
				// 	// Show whatever device has the strongest signal
				// 	curRssi = scanResults[ii].rssi();
				// 	curColorCode = float_num;
				// }
			}
			
		}
	}
	
	// Serial.print("string");
	lcd->setCursor(0,1);
	// sprintf(buffer,"%.2f", float_num);
	// Serial.println(float_num,1);
	// float x = 5.5;
    // std::string result = std::to_string(x);

	// float float_num2 = 5.5;
	char buffer[10]; // Adjust buffer size as needed
	snprintf(buffer, sizeof(buffer), "Lux:%.2f", float_num);
	lcd->print(buffer);

	// lcd->print(3);
	// Log.info("LCD?");
    // lcd->print(Time.hour() < 10? "   0" : "    ");
    // lcd->print(Time.hour());
    // lcd->print(Time.minute() < 10? ":0": ":");
    // lcd->print(Time.minute());
    // lcd->print(Time.second() < 10? ":0": ":");
    // lcd->print(Time.second());
    // lastSecond = Time.second();
	
	Log.info("%d, %f\n", movement, float_num);
} 
