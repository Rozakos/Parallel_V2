#include <Arduino.h>

//?a1&d&s1&c2&s3&c4&s5&c6&s7&c8!


int LED12 = 12;
int LED11 = 11;
int LED10 = 10;
int LED9 = 9;
int LED8 = 8;
int LED7 = 7;
int LED6 = 6;
int LED5 = 5;






#define DEBUG
#ifdef DEBUG
 #define DEBUG_PRINTLN(x)  Serial << (x) << EndLiine;
 #define DEBUG_PRINT(x)  Serial << (x);
#else
 #define DEBUG_PRINTLN(x)
 #define DEBUG_PRINT(x)
#endif 

#define LED 13

//*********************************************************************************
//***************************  Comunication Set  **********************************
//*********************************************************************************
char c;
byte index = 0;
String masege[10]={};
boolean reading = false; //Char Read
boolean getInputComplete = false;
boolean getComand = false;
#define SerialBoud 115200 // Tsxita Seriaki
#define Delimiter '&' // Diaxoristis gia endoles {38}
#define SubDelimiter '#' // Diaxoristis gia endoles {35}
#define StartInput '?' // Pos Ksekinai i grami  {63}
#define EndInput '!' // Pos tlioni i grami  {33}
#define ID "a1"    // ID tis siskevis
#define EndLiine "\r\n"//"\n\r" // 

#define USE_SERIAL
//#define USE_ETH

#define ONELES {int len = masege[index].length();masege[index] = masege[index].substring(0, len -1);} //Za Backspase i Delite 
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }  //Print Class <<
//*********************************************************************************
//***************************  Comunication Set  **********************************
//*********************************************************************************

void setup() 
{
	Serial.begin(SerialBoud);
	DEBUG_PRINT("Starting - ");
	DEBUG_PRINTLN(ID);
	//Serial << F(ID) << EndLiine;
	//Serial << F(">") ;
	Serial.flush();
	pinMode(LED, OUTPUT);
	pinMode(LED12, OUTPUT);
	pinMode(LED11, OUTPUT);
	pinMode(LED10, OUTPUT);	
	pinMode(LED9, OUTPUT);
	pinMode(LED8, OUTPUT);
	pinMode(LED7, OUTPUT);
	pinMode(LED6, OUTPUT);
	pinMode(LED5, OUTPUT);
}



//*********************************************************************************
//***************************  Comunication void **********************************
//*********************************************************************************

void getInput()
{
	switch (c) 
	{
		case 32 :  //  space
		
		break;
		case 8 :  //  Backspace
			ONELES;
		break;
		case 127 :  // Delete 
			ONELES;
		break;
		case Delimiter:
			
			//Serial << F("Got Mesage : ") << index << EndLiine;
			DEBUG_PRINT("Got Mesage : ");
			DEBUG_PRINTLN(index);
			DEBUG_PRINTLN(masege[index]);
			index++;
		break;
		case EndInput :
			reading = false;
			getInputComplete = true;
			//Serial << F("Got Line") << EndLiine;
			DEBUG_PRINTLN("End Mesages");
		break;
			case 13 :  //  Enter
			reading = false;
			getInputComplete = true;
			//Serial << F("Got Line") << EndLiine;
			DEBUG_PRINTLN("Got Line");
		break;
		default:
			
			masege[index] += c;
			
	}
}

void communicationEvent() 
{
	#if defined USE_SERIAL
		while(Serial.available()) 
		{
			c = Serial.read();
			if(c == '?') 
			{
				reading = true;
				c = ' ';
			}
			if(reading)
			{
				getInput();
			}
		}
	#endif 
	#if defined USE_ETH
		c = client.read();
			if(c == '?') 
			{
				reading = true;
				c = ' ';
			}
			if(reading)
			{
				getInput();
			}
	#endif 
	
}



void eraseInput()
{
	for (int i = 0; i <= index; i++)
	{
		masege[i]=""; 
	}
	Serial.flush();
	
	index = 0;
	getInputComplete = false;
	getComand = false;
}

void softReset() 
{
	
	asm volatile ("  jmp 0");
	
} 

void printID()
{
	#if defined USE_SERIAL
		Serial << StartInput <<F(ID) << EndInput << EndLiine ;
	#endif	
	
	#if defined USE_ETH
		client.println(ID);
		client.print("<br>");
	#endif
}


void setDO(int pin ,int State )
{
	pin = pin + 4;
	digitalWrite(pin , State);
}

void invertDO(int pin)
{
	pin = pin + 4;
	int Stat = digitalRead(pin);
	digitalWrite(pin , !Stat);
}

void ReadDO(int pin)
{
	pin = pin + 4;
	int Stat = digitalRead(pin);
	#if defined USE_SERIAL
		Serial << F("Status - ") << Stat  << EndLiine ;
	#endif	
	
	#if defined USE_ETH
		client.println(Stat);
		client.print("<br>");
	#endif
}


void prosesInput()
{
	if (masege[0] == ID)
	{
		//Serial << F("Got ID") << EndLiine ;
		DEBUG_PRINTLN("Got ID");
		getComand = true; 
	}
	else if (masege[0] == "731215")  //Codiko
	{
		printID();
		eraseInput();
	}
	else 
	{
		getComand = false;
		Serial << F("???") << EndLiine ;
		eraseInput();  //???
	}
}

//*********************************************************************************
//********************************** Comunication end *****************************
//*********************************************************************************

void prosesComand()
{
	 if (masege[1] == "eho")
	{
		printID() ;
	}
	 if (masege[1] == "t1")
	{
		Serial << F("25") << EndLiine ;
	}
	else if (masege[1] == "d")
	{
		 for (int i = 2; i <= index; i++)
		{
			if (masege[i].startsWith("s"))
			{
				int pindo = masege[i].substring(1).toInt() ;
				
				
				setDO (pindo , HIGH);
				
				
			}
			if (masege[i].startsWith("c"))
			{
				int pindo = masege[i].substring(1).toInt() ;
				
				setDO (pindo , LOW);
			}
			if (masege[i].startsWith("i"))
			{
				int pindo = masege[i].substring(1).toInt() ;
				
				invertDO (pindo );
			}
			if (masege[i].startsWith("r"))
			{
				int pindo = masege[i].substring(1).toInt() ;
				
				ReadDO (pindo );
			}
		}
		//refreshDO();		
	}
	else if (masege[1] == "reboot")
	{
		//Serial << F("end") << EndLiine ;
		DEBUG_PRINTLN("end ...");
		softReset();
	}

}
   


void loop() 
{
//*********************************************************************************
//***************************  Comunication Loop  *********************************
//*********************************************************************************
	communicationEvent() ;
	if(getInputComplete) {prosesInput();if(getComand){prosesComand();}eraseInput();Serial << F("q") << EndLiine;}
//*********************************************************************************
//***************************  Comunication Loop  *********************************
//*********************************************************************************


}
