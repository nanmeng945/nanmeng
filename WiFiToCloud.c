#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "delay.h"
#include "WiFiToCloud.h"


int8_t ESP8266_SendCount(int count)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//????
	sprintf((char *)TxetBuf,"{\"t\":3,\"datatype\":1,\"datas\":{\"GarbageCan_Count\":%d},\"msgid\":001}",count);
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//????
		error=-1;
	}
	else
	{//????
		for(TryGo = 0; TryGo<50; TryGo++)//??????50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") == NULL)//?????????"????"
			{
				error = -1;
			}
			else
			{
				error = 0;
				break;
			}
			DelayMs(10);
		}
	}
	return error;
}

int8_t ESP8266_SendFault()
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//????

	sprintf((char *)TxetBuf,"{\"t\":3,\"datatype\":1,\"datas\":{\"GarbageCan_Fault\":true},\"msgid\":001}");
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//????
		error=-1;
	}
	else
	{//????
		for(TryGo = 0; TryGo<50; TryGo++)//??????50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") == NULL)//?????????"????"
			{
				error = -1;
			}
			else
			{
				error = 0;
				break;
			}
			DelayMs(10);
		}
	}
	return error;
}

int8_t ESP8266_SendFaultValue(uint8_t fault)
{
    uint8_t TryGo = 0;
    int8_t error = 0;
    uint8_t TxetBuf[MAX_AT_TX_LEN];

    memset(TxetBuf, 0x00, MAX_AT_TX_LEN);
    sprintf((char *)TxetBuf,
            "{\"t\":3,\"datatype\":1,\"datas\":{\"GarbageCan_Fault\":%s},\"msgid\":001}",
            fault ? "true" : "false");

    if (ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0) {
        error = -1;
    } else {
        for (TryGo = 0; TryGo < 50; TryGo++) {
            if (strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") == NULL) {
                error = -1;
            } else {
                error = 0;
                break;
            }
            DelayMs(10);
        }
    }

    return error;
}

/*******************************************************************
*??:int8_t ESP8266_SetStation(void)
*??:ESP8266???station??
*??:?
*??:
		return = 0 ,sucess
		return < 0 ,error
*????:
*******************************************************************/
int8_t ESP8266_SetStation(void)
{
	ClrAtRxBuf();//????
	SendAtCmd((uint8_t *)AT_CWMODE,strlen(AT_CWMODE));
	printf("1 AT+CWMODE_CUR=1\r\n");
	DelayMs(100);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	return 0;
}

/*******************************************************************
*??:int8_t ESP8266_SetAP(void)
*??:??ESP8266???????????
*??:char *wifi-???? char *pwd-????
*??:
		return = 0 ,sucess
		return < 0 ,error
*????:
*******************************************************************/
int8_t ESP8266_SetAP(char *wifi, char *pwd)
{
	uint8_t AtCwjap[MAX_AT_TX_LEN];
	memset(AtCwjap, 0x00, MAX_AT_TX_LEN);//????
	ClrAtRxBuf();//????
	sprintf((char *)AtCwjap,"AT+CWJAP_CUR=\"%s\",\"%s\"",wifi, pwd);
	printf("2 %s\r\n",AtCwjap);////////////////////////////////////////////////////////////
	//printf("AT+CWJAP_CUR=newland-edu1,12345678 \r\n");
	SendAtCmd((uint8_t *)AtCwjap,strlen((const char *)AtCwjap));
	DelayMs(5500);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	return 0;
}

/*******************************************************************
*??:int8_t ESP8266_SetStation(void)
*??:ESP8266??TCP??
*??:
	char *IpAddr-IP??,??:120.77.58.34
	uint16_t port-???,??0~65535
*??:
		return = 0 ,sucess
		return < 0 ,error
*????:
*******************************************************************/
int8_t ESP8266_IpStart(char *IpAddr, uint16_t port)
{
			 uint8_t IpStart[MAX_AT_TX_LEN];  
			 memset(IpStart, 0x00, MAX_AT_TX_LEN);//????  
			 ClrAtRxBuf();//????  
			 sprintf((char *)IpStart,"AT+CIPSTART=\"TCP\",\"%s\",%d",IpAddr, port);  
			 printf("3 %s\r\n",IpStart);////////////////////////////////////////////////////////////  
			 SendAtCmd((uint8_t *)IpStart,strlen((const char *)IpStart));  
   		 DelayMs(1500);   
    		 if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)  
		   {   
		     return -1;  
   		 }  
		 return 0;  
		
}

/*******************************************************************
*??:int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
*??:ESP8266????
*??:
		char *IpBuf-IP??
		uint8_t len-????
*??:
		return = 0 ,sucess
		return < 0 ,error
*????:
*******************************************************************/
int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
{
	 uint8_t TryGo = 0;   
	 int8_t error = 0;  
 	uint8_t IpSend[MAX_AT_TX_LEN];  
  memset(IpSend, 0x00, MAX_AT_TX_LEN);//????   
	ClrAtRxBuf();//????   
	sprintf((char *)IpSend,"AT+CIPSEND=%d",len);  
	printf("4 %s\r\n",IpSend);////////////////////////////////////////////////////////////  
	SendAtCmd((uint8_t *)IpSend,strlen((const char *)IpSend));   
    DelayMs(1500);   
    if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)   
   {  
         return -1;   
    }  
	 ClrAtRxBuf();//????  
		SendStrLen((uint8_t *)IpBuf, len);  
    printf("%s\r\n",IpBuf);////////////////////////////////////////////////////////////   
		for(TryGo = 0; TryGo<60; TryGo++)//?????? 100*60=6000ms  
		{
		 if(strstr((const char *)AT_RX_BUF, (const char *)"SEND OK") == NULL)  
		 {  
		 error = -2;   
		 }   
		 else  
		 {   
     		error = 0;   
		    break;  
		 }   
		DelayMs(100); 
		    }  
    return error;  
}

/*******************************************************************
*??:int8_t ConnectToServer(void)
*??:??????
*??:?
*??:
		return = 0 ,sucess
		return < 0 ,error
*????:
*******************************************************************/
int8_t ConnectToServer(char *DeviceID, char *SecretKey)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//????
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_SetStation() == 0)//??WiFi????????
		{
			error = 0;
			break;
		}
		else
		{
			error = -1;
		}
	}
	if(error < 0)
	{
		return error;
	}
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_SetAP((char *)WIFI_AP, (char *)WIFI_PWD) == 0)//?????????
		{
			error = 0;
			break;
		}
		else
		{
			error = -2;
		}
	}
	if(error < 0)
	{
		return error;
	}
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_IpStart((char *)SERVER_IP,SERVER_PORT) == 0)//?????IP??,??:120.77.58.34,8600
		{
			error = 0;
			break;
		}
		else
		{
			error = -3;
		}
	}
	if(error < 0)
	{
		return error;
	}
	
	sprintf((char *)TxetBuf,"{\"t\":1,\"device\":\"%s\",\"key\":\"%s\",\"ver\":\"v0.0.0.0\"}",DeviceID,SecretKey);
	//printf("%s\r\n",TxetBuf);////////////////////////////////////////////////////////////
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//????
		error=-4;
	}
	else
	{//????
		for(TryGo = 0; TryGo<50; TryGo++)//??????50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") == NULL)//?????????????
			{
				error = -5;
			}
			else
			{
				error = 0;
				break;
			}
			DelayMs(10);
		}
	}
	
	return error;
}

/*******************************************************************
*??:int8_t ESP8266_SendSensor(uint8_t sensor)
*??:??????????
*??:
	uint8_t sensor, ????
	char *TimeStr,????????,??yyyy-mm-dd hh:mm:ss??
*??:
		return = 0 ,sucess
		return < 0 ,error
*????:
*******************************************************************/
int8_t ESP8266_SendSensor(uint8_t sensor, char *TimeStr)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//????
	sprintf((char *)TxetBuf,"{\"t\":3,\"datatype\":2,\"datas\":{\"alarm\":{\"%s\":%d}},\"msgid\":001}",TimeStr,sensor);
	//printf("%s\r\n",TxetBuf);////////////////////////////////////////////////////////////
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//????
		error=-1;
	}
	else
	{//????
		for(TryGo = 0; TryGo<50; TryGo++)//??????50*10=500ms
		{
			if(strstr((const char *)AT_RX_BUF, (const char *)"\"status\":0") == NULL)//?????????"????"
			{
				error = -1;
			}
			else
			{
				error = 0;
				break;
			}
			DelayMs(10);
		}
	}
	return error;
}

/*******************************************************************
*??:uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
*??:???????
*??:
		uint8_t *AtRxBuf ,??AT????
		char *GetIpData ,?????Itcp/ip??
*??:?????IP????
*????:
	AT+CIPSEND=76
	{"t":3,"datatype":2,"datas":{"alarm":{"2018-06-19 18:15:02":0}},"msgid":001}
	+IPD,29:{"msgid":1,"status":0,"t":4}
*******************************************************************/
uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
{
	char *Point = NULL;
	uint8_t len = 0;
	
	Point = strstr((const char *)AtRxBuf, (const char *)"+IPD,");
	if(Point != NULL)//??????TCP/IP????
	{
		len=(Point[5]-'0')*10+Point[6]-'0';//??IP????????100,????“??”?“??”?????IP????
		Point = strstr((const char *)AtRxBuf, (const char *)":")+1;
		memcpy(GetIpData, Point, len);
		GetIpData[len]='\0';
		printf("??IP??:%s\r\n",GetIpData);///////////////////////////////////////////////////////
		printf("??IP????=%d\r\n",len);///////////////////////////////////////////////////
	}
	
	return (len);
}

/*******************************************************************
*??:void ESP8266_DataAnalysisProcess(char *RxBuf)
*??:???????
*??:char *RxBuf ???????
*??:
*????:?????????????????,????????DEMO
*******************************************************************/
void ESP8266_DataAnalysisProcess(char *RxBuf)
{
	if(strstr((const char *)RxBuf, (const char *)PING_REQ) != NULL)//?????
	{
		ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP));//????
	}
	else if(strstr((const char *)RxBuf, (const char *)"\"t\":5") != NULL)//?????
	{
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"LED2_Exec\"") != NULL)//??LED2???
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//?LED2
			{
				//PlatformStatus = PLATFROM_CTRL_ON;
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//?LED2
			{
				//PlatformStatus = PLATFROM_CTRL_OFF;
				
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"defense\"") != NULL)//??/????
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//??
			{
				printf("??!\r\n");
				;//...
				;//...
				;//...
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//??
			{
				printf("??!\r\n");
				;//...
				;//...
				;//...
			}
		}
	}
}