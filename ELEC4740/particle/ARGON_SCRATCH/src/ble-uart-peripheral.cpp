#include "Particle.h"
//Function Def
uint16_t Sound();
uint16_t Push_Button();
float Temperature();
void Fan_Control();

SerialLogHandler logHandler; 


// // Timers
// void Led_1_Startup();
// void Led_2();


// Timer Time_1(500, Led_1_Startup);
// Timer Time_2(500, Led_2);


// Pins
// Analog Pins
int PWM_FAN = A3;
int POT = A4;
int TEMP_INPUT = A5;
// Digital Pins
int SOUND_INPUT = D0;
int LED_TWO_GREEN = D1;
int LED_ONE_RED = D2;
int LED_ONE_GREEN = D3; 
int LED_TWO_RED = D4;
int BUTTON  = D5; 
// Values
int Pot_Read = 0;
int RPM_Read = 0;
bool Button_State = TRUE;
const int debounceTime = 100; // Debounce time in milliseconds (adjust as needed)
unsigned long lastDebounceTime = 0;  // Last time the button state change was detected
float Total_temp = 0.0;
// BlueTooth
const unsigned long UPDATE_INTERVAL_MS = 2000; 
unsigned long lastUpdate = 0; 
float Temp_S;
uint16_t Sound_S;
uint16_t Button_S;
uint8_t check = 0xcc;


// Setting up Severives
const BleUuid serviceUuid("bfc37fc6-64b1-4f2d-a3e5-c0df90bd5cdd");
const BleUuid TempUuid("e503d4f7-ad9a-4131-a192-7150b3a7fc78");
// const BleUuid SoundUuid("06ddddf7-e00d-4ee8-83eb-bb7317c8589b");
const BleUuid CentralRx01("11ba35cc-8edb-49a0-ac68-375dee5f77ff");
const size_t TempUuid_BUF_SIZE = 8;
const size_t SoundUuid_BUF_SIZE = 2;
const size_t ButtonUuid_BUF_SIZE = 2;
BleCharacteristic temperatureMeasurementCharacteristic("temp", BleCharacteristicProperty::NOTIFY, TempUuid, serviceUuid);
BleCharacteristic soundMeasurementCharacteristic("CentralRx01", BleCharacteristicProperty::NOTIFY, CentralRx01, serviceUuid);
BleCharacteristic peerCentralRx01;
BlePeerDevice peer;
uint16_t count;


void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context)
{
 memcpy(&count,&data,2);
 Log.info("count = %d",count);


}






void setup()
{
 //(void)logHandler; // Does nothing, just to eliminate the unused variable warning
 Serial.begin(9600); //particle serial monitor --follow >code to put into bash
 waitFor(Serial.isConnected, 30000);
 //Output Pins
 pinMode(LED_ONE_RED,OUTPUT);
 pinMode(LED_ONE_GREEN,OUTPUT); 
 pinMode(LED_TWO_RED,OUTPUT);
 pinMode(LED_TWO_GREEN,OUTPUT);
 pinMode(PWM_FAN, OUTPUT);
 Serial.println("Output Pins Set");
 //Input Pins
 pinMode(TEMP_INPUT, INPUT);
 pinMode(SOUND_INPUT, INPUT);
 pinMode(BUTTON, INPUT);
 pinMode(POT,INPUT);
 Serial.println("Input Pins Set");
 //Start Needed Timers
 // Time_1.start();
 // BlueTooth
 BLE.on();
 BLE.addCharacteristic(temperatureMeasurementCharacteristic);
 // BLE.addCharacteristic(soundMeasurementCharacteristic);
 // BLE.addCharacteristic(buttonMeasurementCharacteristic);
 peerCentralRx01.onDataReceived(onDataReceived, &peerCentralRx01);
 BleAdvertisingData data;
 data.appendServiceUUID(serviceUuid);
 BLE.advertise(&data);
}


void loop()
{
 if(BLE.connected())
 {
     uint8_t Temp_Buf[TempUuid_BUF_SIZE];
     // uint8_t Sound_Buf[SoundUuid_BUF_SIZE];
     // uint8_t Button_Buf[ButtonUuid_BUF_SIZE];


     Temp_S = Temperature();
     Sound_S = Sound();
     Button_S = Push_Button();


     // Serial.println(Temp_S);
     // Serial.println(Sound_S);
     // Serial.println(Button_S);


     memcpy(&Temp_Buf[0], &Temp_S, 4);
     memcpy(&Temp_Buf[4], &Sound_S, 2);
     memcpy(&Temp_Buf[6], &Button_S, 2);


     temperatureMeasurementCharacteristic.setValue(Temp_Buf, sizeof(Temp_Buf));
     // soundMeasurementCharacteristic.setValue(Sound_Buf, sizeof(Sound_Buf));
     // buttonMeasurementCharacteristic.setValue(Button_Buf, sizeof(Button_Buf));


 }
}


uint16_t Push_Button()
{
 uint16_t Read_Button = digitalRead(BUTTON);
 unsigned long currentTime = millis();
 // Check if enough time has passed since the last debounce
 if ((currentTime - lastDebounceTime) >= debounceTime) {
   // Update button state only if the reading has been stable for the debounce time
   if (Read_Button != Button_State) {
     Button_State = !Button_State; // Invert button state
     lastDebounceTime = currentTime; // Reset debounce timer
   }
 }
 if(Button_State == TRUE)
 {
   Read_Button = 1;
 }
 else if (Button_State == FALSE)
 {
   Read_Button = 0;
 }
 return Read_Button;
}   


// Push Button Alarm Timer Function
void Push_Button_Alarm()
{ 
 
}


uint16_t Sound()
{
 int Read_Sound = digitalRead(SOUND_INPUT);
 int S;
 if(Read_Sound == 0){
   S = 1;
 }
 else
 {
   S = 0;
 }
 return S;
} 


float Temperature()
{
for(int i = 0; i < 100; i++)
 {
   float adcData = analogRead(TEMP_INPUT);
   float voltage = adcData * (3.3 / 4095.0);
   float temperature = (voltage * 100);
   Total_temp = Total_temp + temperature;
 } 
 float Total_avg = Total_temp/100;
 Total_temp = 0;
 return Total_avg;
}


void Fan_Control()
{
Pot_Read = analogRead(POT)/16;
double Temp_Read = Temperature();
if(Temp_Read >= 40.0)
{
 analogWrite(PWM_FAN, 255);
}
else
{
   if(Pot_Read < 5)
  {
     Pot_Read = 0;
  }
 else if(Pot_Read > 255)
 {
     Pot_Read = 255;
 }
 analogWrite(PWM_FAN, Pot_Read);
 }  
}
