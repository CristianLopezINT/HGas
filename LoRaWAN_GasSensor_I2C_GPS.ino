#include "AiThinker_LoRaWAN_cfg.h"

#include <CayenneLPP.h>

#include <WDTZero.h>

#include <Wire.h>

#include <AS5600.h>

#include <NMEAGPS.h>
#include <GPSport.h>

//Varibles for radiowaves data
short rssi;
short len = 0;
extern char buff[256]; 

//Create objects
WDTZero MyWatchDoggy; // Define WDT   
CayenneLPP lpp(51);     //Cayenne encoding object, maypayload of 51 bytes for 125Khz channels
AMS_5600 ams5600;
NMEAGPS  gps; // This parses the GPS characters
gps_fix  fix; // This holds on to the latest values


void setup() 
{
  SerialUSB.begin(115200); //Serial Monitor
  //setupLoRaABP();
  Setup_LoRa();
  Setup_GPS();
  Setup_GasSensor();
  Setup_Pins();
  Setup_WDT();
}

void loop() 
{ 
  lpp.reset();
  GasSensor_Read();
  GPS_Read();
  Send_Data();
  
  MyWatchDoggy.clear();  // refresh wdt - before it loops    
  digitalWrite(LED_BUILTIN, LOW);
 
}


void Send_Data()
{
  bool state = false;
  
  digitalWrite(LED_BUILTIN, HIGH);
  
  state = sendCayenneEncodedData();
  
  if(state == false)
  {
    lora.setDeviceReset();    
  }
}

void GasSensor_Read()
{
  float percentage;
  percentage = convertRawAngleToDegrees(ams5600.getRawAngle());
  lpp.addAnalogInput(1, percentage);
  SerialUSB.println(percentage);
}

float convertRawAngleToDegrees(word newAngle)
{
  int ang = 0;
  float retVal = newAngle * 0.087; 
  ang = retVal - 180;   //Angle
  float x3 =  pow(ang, 3);
  float per = 50 + (0.475*ang)-(0.00000762*x3); //Percentage
  return per;
}

void GPS_Read()
{
  float latitude;
  float longitude;
  float altitude;
  int satellites;

  bool gpsflag = true;
  while(gpsflag==true)
  {
    while (gps.available( gpsPort )) 
    {
      fix = gps.read();
      
      if (fix.valid.location) 
      {
        latitude = fix.latitude();
        longitude = fix.longitude();
        DEBUG_PORT.print( F("Location: ") );
        DEBUG_PORT.print( latitude, 6 );
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( longitude, 6 );
      }
  
      if (fix.valid.altitude)
      {
        altitude = fix.altitude();
        DEBUG_PORT.print( F(", Altitude: ") );
        DEBUG_PORT.println( altitude );
      }
      
      if (fix.valid.satellites)
      {
        satellites = fix.satellites;
        DEBUG_PORT.print("Satelites: ");
        DEBUG_PORT.print( satellites );
      }
      else
      {
        satellites = 0;
      }
      gpsflag = false;
    }
  }
  
  DEBUG_PORT.println();
  gpsflag = true;

  lpp.addGPS(2, latitude, longitude, altitude); 
  lpp.addDigitalOutput(3, satellites); 
}

void Setup_GPS()
{
  Serial1.begin(9600);
}

void Setup_GasSensor()
{
  Wire.begin();
}

void Setup_Pins()
{
  // Initialize pin LED_BUILTIN as an output
  pinMode(14, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void Setup_LoRa()
{ 
  lora.setDeviceReset();
  lora.init();
}

void Setup_WDT()
{
  MyWatchDoggy.attachShutdown(myshutdown);     
  MyWatchDoggy.setup(WDT_HARDCYCLE16S);  // initialize WDT-softcounter refresh cycle on 8m interval
  MyWatchDoggy.clear();  // refresh wdt - before it loops       
}

void myshutdown()
{

  SerialUSB.print("\nWe gonna shut down ! ...");
  
}

bool sendCayenneEncodedData(){
  bool state;
  state = lora.transferPacket(lpp.getBuffer(), lpp.getSize(), DEFAULT_RESPONSE_TIMEOUT);
  delay(50);
  frame_counter++;
        
  memset(buff, 0, 256);
  len = lora.receivePacket(buff, 256, &rssi);
        
  if(len)
  {
    SerialUSB.print("Length is: ");
    SerialUSB.println(len);
    SerialUSB.print("RSSI is: ");
    SerialUSB.println(rssi);
    SerialUSB.print("Data is: ");
    
    for(unsigned char i = 0; i < len; i ++)
    {
      //SerialUSB.print("0x");
      //SerialUSB.print(buff[i], HEX);
      SerialUSB.print((char)buff[i]);
      SerialUSB.println(" ");
    }
      
  }
  return state;
}
