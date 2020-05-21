#ifndef AITHINKER_LORAWAN_CFG_H
#define AITHINKER_LORAWAN_CFG_H

#include "Arduino.h"
#include "LoRaWAN_Hnb.h"
#include "keys_generic.h"

//Definitions for LoRaWAN settings
#define FREQ_RX_WNDW_SCND_US  923.3
const float US_hybrid_channels[8] = {903.9, 904.1, 904.3, 904.5, 904.7, 904.9, 905.1, 905.3}; 
#define DOWNLINK_DATA_RATE_US DR1
#define US_RX_DR DR8
#define UPLINK_DATA_RATE_MAX_US DR3
#define UPLINK_DATA_RATE_MIN DR1
#define MAX_EIRP_NDX_US 14            //The number means the quantity in dBi
#define DEFAULT_RESPONSE_TIMEOUT 4


//LoRaWanClass lora;

int loopcount = 0; //Needed when OTAA enabled
unsigned char frame_counter = 1;

char buff[256] = {0}; 

void setHybridForTTN(const float* channels){
    for(int i = 0; i < 8; i++){
        if(channels[i] != 0){
            lora.setChannel(i, channels[i], UPLINK_DATA_RATE_MIN, UPLINK_DATA_RATE_MAX_US);        
        }
    }
}

void setupLoRaABP(){
    lora.init();
    //lora.setDeviceDefault();
    
    lora.setDeviceReset();
    
    memset(buff, 0, 256);
    lora.getVersion(buff, 256, 1);
    SerialUSB.print(buff); 
       
    memset(buff, 0, 256);
    lora.getId(buff, 256, 1);
    SerialUSB.print(buff);
    
    lora.setId(DEV_ADDR, DEV_EUI, APP_EUI);
    lora.setKey(NWK_S_KEY, APP_S_KEY, APP_KEY);

    //lora.setDutyCycle(false);
    
    lora.setDeviceMode(LWABP);
    //lora.setClassType(CLASS_C);         //RisingHf device modem is Class A by default
    lora.setDataRate(DR1, US915HYBRID);
    lora.setPower(MAX_EIRP_NDX_US);  //Set to 13 dBi
    lora.setPubNetwKey(1);
    setHybridForTTN(US_hybrid_channels);
    lora.setReceiveWindowFirst(1);
    //lora.setReceiveWindowFirst(0,  US_hybrid_channels[0]);
    lora.setReceiveWindowSecond(FREQ_RX_WNDW_SCND_US, DR1);
}

void setupLoRaOTAA(){   
    
    lora.init();
    lora.setDeviceReset();
  
    memset(buff, 0, 256);
    lora.getVersion(buff, 256, 1);
    SerialUSB.print(buff); 
       
    memset(buff, 0, 256);
    lora.getId(buff, 256, 1);
    SerialUSB.print(buff);

    lora.setId(DEV_ADDR, DEV_EUI, APP_EUI);
    lora.setKey(NWK_S_KEY, APP_S_KEY, APP_KEY);
    
    lora.setDeviceMode(LWOTAA);
    lora.setClassType(CLASS_C);         //RisingHf device modem is Class A by default
    lora.setDataRate(DR1, US915HYBRID);
    //lora.setAdaptiveDataRate(true); 
    setHybridForTTN(US_hybrid_channels);
    
    lora.setReceiveWindowFirst(0,  US_hybrid_channels[0]);
    lora.setReceiveWindowSecond(FREQ_RX_WNDW_SCND_US, DOWNLINK_DATA_RATE_US);

    lora.setDutyCycle(false);
    lora.setJoinDutyCycle(false);
    lora.setPower(MAX_EIRP_NDX_US);

    SerialUSB.print("Starting OTTA Join.\n");
    loopcount = 0;
    
    while(true) {
        loopcount++;
        if (lora.setOTAAJoin(JOIN))
            break;
    }
    
    SerialUSB.print("Took ");
    SerialUSB.print(loopcount);
    SerialUSB.println(" tries to join.");
    while(!lora.setOTAAJoin(JOIN));
    //lora.loraDebug();
}


#endif
