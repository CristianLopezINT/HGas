/*
  LoRaWAN_Hnb.h
  2018 Copyright (c) Hunabsys R&D S de RL de CV.  All right reserved.

  Author: Jose Carlos Castro | Earvin Filipini | Cristian Lopez
  Date: 2018-07-15
  
*****************************************************************************
  The MIT License (MIT)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.1  USA
****************************************************************************

*/

#ifndef _LORAWAN_HNB_H_
#define _LORAWAN_HNB_H_

#include <Arduino.h>


#define LoRa                SerialLoRa

#define _DEBUG_SERIAL_      1
#define DEFAULT_TIMEOUT     5 // second
#define DEFAULT_TIMEWAIT    100 // millisecond
#define DEFAULT_DEBUGTIME   1 // second

#define BATTERY_POWER_PIN    A4
#define CHARGE_STATUS_PIN    A5

#define BUFFER_LENGTH_MAX    256


enum _class_type_t { CLASS_A = 0, CLASS_B, CLASS_C };
enum _physical_type_t { EU434 = 0, EU868, US915, US915HYBRID, AU915, AU915OLD, CN470, CN779, AS923, KR920, IN865 };
enum _device_mode_t { LWABP = 0, LWOTAA, TEST };
enum _otaa_join_cmd_t { JOIN = 0, FORCE };
enum _window_delay_t { RECEIVE_DELAY1 = 0, RECEIVE_DELAY2, JOIN_ACCEPT_DELAY1, JOIN_ACCEPT_DELAY2 };
enum _band_width_t { BW125 = 125, BW250 = 250, BW500 = 500 };
enum _spreading_factor_t { SF12 = 12, SF11 = 11, SF10 = 10, SF9 = 9, SF8 = 8, SF7 = 7 };
enum _data_rate_t { DR0 = 0, DR1, DR2, DR3, DR4, DR5, DR6, DR7, DR8, DR9, DR10, DR11, DR12, DR13, DR14, DR15 };


//Class interface
class LoRaWanClass
{
    public:
    
        LoRaWanClass(void);
        
        //Virtual destructor
        virtual ~LoRaWanClass() {}
		
        void init(void);
        
        /**@brief Function for testing  */
        void TestCommand(void);
        
        /**@brief Function for getting the firmware version
         *  
  		 *  \param [in] *buffer The data cache
  		 *  \length The length of data cache
  	 	 *  \param [in] timeout The time to get firmware version
 	     * 
         */
        void getVersion(char *buffer, short length, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**@brief Function for getting the device ID
         *  
  		 *  \param [in] *buffer The data cache
  		 *  \length The length of data cache
  	 	 *  \param [in] timeout The time to get device ID
 	     * 
         */
        void getId(char *buffer, short length, unsigned char timeout = DEFAULT_TIMEOUT);
        
		/**@brief function for setting a new device ID
         *  
  		 *  \param [in] *DevAddr device address
  		 *  \param [in] *DevEUI device extended unique identifier
  	 	 *  \param [in] *AppEUI application extended unique identifier
 	     * 
         */
        void setId(char *DevAddr, char *DevEUI, char *AppEUI);
        
        /**@brief Function for setting the network public or private
		 *
         *  \param [in] PubNetwKey Public network key
         */
        void setPubNetwKey(int PubNetwKey);
        
		/**@brief Function for setting the new device Keys  
		 *  
  		 *  \param [in] *NwkSKey network session key
  		 *  \param [in] *AppSKey application session key
  	 	 *  \param [in] *AppKey application key
 	     * 
         */
        void setKey(char *NwkSKey, char *AppSKey, char *AppKey);
        
        /**@brief Function for setting datarate of LoRaWAN AT modem 
		 *     
  		 *  \param [in] datarate the datarate of LoRaWAN AT modem
  		 *  \param [in] physicalType datarate scheme
 	     * 
         */    
        void setDataRate(_data_rate_t dataRate = DR1, _physical_type_t physicalType = US915HYBRID);
		 
        /**@brief Function for setting Adaptative Datarate
		 *  
         *  \param [in] command TRUE/FALSE Adaptative datarate 
 	     * 
         */
        void setAdaptiveDataRate(bool command);
        
        /**@brief Function for setting the output power 
         * 
         *  \param [in] power The LoRaWAN TX power
 	     * 
         */
        void setPower(short power);
        
        /**@brief Function for setting the PORT number which will be used to send message, range from 1 to 255.
         *
         *  \param [in] port The port number 
 	     * 
         */
        void setPort(unsigned char port);
        
        /**@brief Function for setting channel parameter of LoRaWAN modem 
         *
         *  \param [in] channel The channel number, The channel number range from 0 to 71
		 *  \param [in] frequency The frequency to work in 
 	     * 
         */
        void setChannel(unsigned char channel, float frequency);
        
        /**@brief Function for setting channel parameter of LoRaWAN modem
		 * 
         * The channel number range from 0 to 71. Set frequecy zero to disable one channel
         *
         *  \param [in] channel The channel number
		 *  \param [in] frequency The frequency to work in
		 *  \param [in] datarate The datarate to work in
 	     * 
         */
        void setChannel(unsigned char channel, float frequency, _data_rate_t dataRate);
        
        /**@brief Function for setting channel parameter of LoRaWAN modem 
         *
         *  \param [in] channel The channel number
		 *  \param [in] frequency The frequency to work in
		 *  \param [in] dataRateMin The minimum datarate to work in
		 *  \param [in] dataRateMin The maximum datarate to work in
 	     * 
         */
        void setChannel(unsigned char channel, float frequency, _data_rate_t dataRateMin, _data_rate_t dataRateMax);
        
        /**@brief Transfer the data. True : transfer done, false : transfer failed. 
		 * Use to send string format frame which is no need to be confirmed by the server.
		 *  
  		 *  \param [in] *buffer The transfer data cache
  	 	 *  \param [in] timeout The over time of transfer
 	     *   
		 */
        bool transferPacket(char *buffer, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**@brief Transfer the data. True : transfer done, false : transfer failed. 
		 * Use to send hex format frame which is no need to be confirmed by the server.
		 *  
  		 *  \param [in] *buffer The transfer data cache
  		 *  \length The length of data cache
  	 	 *  \param [in] timeout The over time of transfer
 	     *   
		 */
        bool transferPacket(unsigned char *buffer, int length, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**@brief Transfer the data. True : transfer done, false : transfer failed. 
		 * Use to send string format frame which must be confirmed by the server.
		 *  
  		 *  \param [in] *buffer The transfer data cache
  	 	 *  \param [in] timeout The over time of transfer
  	 	 *  
		 *  \return Return bool. Ture : Confirmed ACK, false : Confirmed NOT ACK   
		 */
        bool transferPacketWithConfirmed(char *buffer, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**@brief Transfer the data. True : transfer done, false : transfer failed. 
		 * Use to send hex format frame which must be confirmed by the server.
		 *  
  		 *  \param [in] *buffer The transfer data cache
  		 *  \length The length of data cache
  	 	 *  \param [in] timeout The over time of transfer
 	     *   
		 */
        bool transferPacketWithConfirmed(unsigned char *buffer, unsigned char length, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**@brief function for opening a window to receive data and read it.  
        *  
  		*  \param [in] *buffer The received data cache
     	*  \length The length of data cache
  	    *  \param [in] *rssi The RSSI cache
  	    *
 	    *  \return Return Receive data number 
		*/
        short receivePacket(char *buffer, short length, short *rssi);
        
        /**
 		 *  @brief Transfer the proprietary data
 		 *  
  		 *  \param [in] *buffer The transfer data cache
  	 	 *  \param [in] timeout The over time of transfer
 	     *  
         *  \return Return bool. Ture : transfer done, false : transfer failed
         */
        bool transferProprietaryPacket(char *buffer, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**
 		 *  @brief Transfer the proprietary data
 		 *  
  		 *  \param [in] *buffer The transfer data cache
  		 *  \length The length of data cache
  	 	 *  \param [in] timeout The over time of transfer
 	     *  
         *  \return Return bool. Ture : transfer done, false : transfer failed
         */
        bool transferProprietaryPacket(unsigned char *buffer, unsigned char length, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**@brief function for selecting work mode, LWABP, LWOTAA.  */
        void setDeviceMode(_device_mode_t mode);
        
        /**@brief function to enable OTAA mode and join a known network   
		 *
		 *  \param [in] command The type of join
  	     *  \param [in] timeout The over time of join
         *  
  	     *  \return Return bool. True : join OK, false : join NOT OK
		 */
        bool setOTAAJoin(_otaa_join_cmd_t command, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /**
  		 *  @brief Function for setting message unconfirmed repeat time
  		 *  
  		 *  \param [in] time The repeat time, range from 1 to 15
 		 *  
  		 *  \return Return null
 		 */        
        void setUnconfirmedMessageRepeatTime(unsigned char time);
        
        /**
  		 *  @brief Function for setting message retry time
  		 *  
  		 *  \param [in] time The retry time, range from 0 to 254
  		 *  
  		 *  \return Return null
 		 */
        void setConfirmedMessageRetryTime(unsigned char time);
        
        /**
 		 *  @brief Function for setting ON/OFF receive window 1     
  		 *  
  		 *  \param [in] command The true : ON, false OFF
  		 *  
  		 *  \return Return null
  		 */
        void setReceiveWindowFirst(bool command);
        
        /**
  		 *  @brief Function for setting receive window 1 channel mapping
 		 *  
 		 *  \param [in] channel The channel number, range from 0 to 71
	     *  \param [in] frequency The frequency value of channel
  		 *  
 		 *  \return Return null
  		 */
        void setReceiveWindowFirst(unsigned char channel, float frequency);
        
        /**
  		 *  @brief Function for setting receive window 2 channel mapping
  		 *  
  		 *  \param [in] frequency The frequency value of channel
  		 *  \param [in] dataRate The date rate value
  		 *  
  		 *  \return Return null
  		 */
        void setReceiveWindowSecond(float frequency, _data_rate_t dataRate);
        
        /**
  		 *  @brief Function for setting receive window 2 channel mapping
 		 *  
  		 *  \param [in] frequency value of channel
 		 *  \param [in] spreadingFactor The spreading factor value
 		 *  \param [in] bandwidth The band width value
 		 *  
 		 *  \return Return null
  		 */
        void setReceiveWindowSecond(float frequency, _spreading_factor_t spreadingFactor, _band_width_t bandwidth);
        
        /**
 		 *  @brief Function for setting ON/OFF duty cycle limitation
 		 *  
 		 *  \param [in] command The true : ON, false OFF
 		 *  
 		 *  \return Return null
 		 */
        void setDutyCycle(bool command);
        
        /**
  		 *  @brief Function for setting ON/OFF join duty cycle limitation
  		 *  
  		 *  \param [in] command The true : ON, false OFF
  		 *  
  		 *  \return Return null
 		 */
        void setJoinDutyCycle(bool command);
        
        /**
 		 *  @brief Function for setting receive window delay
  		 *  
  		 *  \param [in] command The delay type
  		 *  \param [in] _delay The delay value(millisecond)
 		 *  
 		 *  \return Return null
 		 */
        void setReceiveWindowDelay(_window_delay_t command, unsigned short _delay);
        
        /** @brief Function for setting LoRaWAN class type A,B,C  
         *  
  		 *  \param [in] type the class type the transceiver will work in
 		 *
		 */
        void setClassType(_class_type_t type);
        
        /** @brief Function for setting device into low power mode */
        void setDeviceLowPower(void);
        
        /** @brief Function to Reset device  */
        void setDeviceReset(void);
        
        /** @brief Funcion to Set up device default */
        void setDeviceDefault(void);
        
        /**
  		 *  @brief Initialize device into P2P mode
  		 * 
 		 *  \param [in] frequency The ISM frequency value
 		 *  \param [in] spreadingFactor The spreading factor value
 		 *  \param [in] bandwidth The band width value
 		 *  \param [in] txPreamble The transfer packet preamble number
 		 *  \param [in] rxPreamble The receive packet preamble number
	     *  \param [in] power The output power
 		 *  
 		 *  \return Return null
 		 */
        void initP2PMode(unsigned short frequency = 915, _spreading_factor_t spreadingFactor = SF9, _band_width_t bandwidth = BW125, 
                         unsigned char txPreamble = 8, unsigned char rxPreamble = 8, short power = 20);
						  
        /* @brief Function for transfering a packet in a P2P Network
 		 *  
  		 *  \param [in] *buffer The transfer data cache
  	 	 *  \param [in] timeout The over time of transfer
 	     *
		 * Return bool. True : transfer done, false : transfer failed  */
        bool transferPacketP2PMode(char *buffer, unsigned char timeout = DEFAULT_TIMEOUT);
		 
        /* @brief Function for transfering a packet in a P2P Network
         *
         *  \param [in] *buffer The transfer data cache
  		 *  \length The length of data cache
  	 	 *  \param [in] timeout The over time of transfer
  	 	 *
		 * Return bool. Ture : transfer done, false : transfer failed  
		 */
        bool transferPacketP2PMode(unsigned char *buffer, unsigned char length, unsigned char timeout = DEFAULT_TIMEOUT);
        
        /* Function for transfering a packet in a P2P Network
		 *  
         *  \param [in] *buffer The transfer data cache
  		 *  \length The length of data cache
  		 *  \param [in] *rssi The RSSI cache
  	 	 *  \param [in] timeout The over time of transfer
  	 	 *
  	 	 */
        short receivePacketP2PMode(unsigned char *buffer, short length, short *rssi, unsigned char timeout = DEFAULT_TIMEOUT);
        
		/* @brief Function for reading loraWAN raw data  */
        void loraDebug(void);
        
#if _DEBUG_SERIAL_
		/* @brief Function for printing the read loraWAN raw data  */
        void loraDebugPrint(unsigned char timeout);  
#endif  
		/* @brief Function for getting the battery voltage  */
        short getBatteryVoltage(void);
        
        
    private:
        void sendCommand(char *command);
        short readBuffer(char* buffer, short length, unsigned char timeout = DEFAULT_TIMEOUT);
        bool waitForResponse(char* response, unsigned char timeout = DEFAULT_TIMEOUT);
        bool sendCommandAndWaitForResponse(char* command, char *response, unsigned char timeout = DEFAULT_TIMEOUT);
        
        char _buffer[256];

};


extern LoRaWanClass lora;

#endif
