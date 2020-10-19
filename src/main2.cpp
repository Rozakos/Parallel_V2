#include <Arduino.h>

//?a1&d&s1&c2&s3&c4&s5&c6&s7&c8!

#define Delimiter ':' // How we separate the command
#define EndInput '\r' // How the input ends
#define StartMessage '!' // How the message starts
#define EndMessage '$' // How the message ends
#define EndLine "\n\r"//"\n\r" // 

int LED12 = 12;
int LED11 = 11;
int LED10 = 10;
int LED9 = 9;
int LED8 = 8;
int LED7 = 7;
int LED6 = 6;
int LED5 = 5;

int timer = 1000;

char incomingByte;
byte index = 0;
char bit_8_array[8] = {3, 3, 3, 3, 3, 3, 3, 3};
String message[30] = "";
boolean startRead = false;
boolean getInputComplete = false;
boolean reading_serial = false;


void setup() {
  // put your setup code here, to run once:
  pinMode(LED12, OUTPUT);
  pinMode(LED11, OUTPUT);
  pinMode(LED10, OUTPUT);
  pinMode(LED9, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED5, OUTPUT);
  Serial.begin(9600);
  
}



void getInput(){

  if (startRead == true)
  {
    if (incomingByte ==  49)
    {
      Serial.println("OK2");
      for (int index = 0; index < 8; index++);{
        if (bit_8_array[index] == 3)
        {
          bit_8_array[index] = 1;
               
        }         
      }
    }
  }
}


// !49



void serial_event()
{
  if(Serial.available() > 0)
  {
    getInput();
  }
}



void communication_event()
{
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    Serial.print("Message received: ");
    Serial.println(incomingByte, DEC);
    }
    if (incomingByte == 63)
    {
      Serial.println("? received");
      reading_serial = true;
      incomingByte = ' ';
    }
    if(reading_serial == true)
    {
      getInput();
    }
  }

  void loop() {
  communication_event();
}
