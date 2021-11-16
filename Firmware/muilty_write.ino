#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Butt";   // your network SSID (name) 
const char* password = "dickdick";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 1502861;
const char * myReadAPIKey = "AZBKPYY7B3KKION9";
const char * myWriteAPIKey = "RX257VSH16FJ63TH";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to hold temperature readings
float temperatureC;

void setup() 
{
  Serial.begin(115200);  //Initialize serial
  
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() 
{
  if ((millis() - lastTime) > timerDelay) 
  {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    int thermistor_value;
    thermistor_value  = analogRead(34);
    thermistor_value = thermistor_value/23.17;
    Serial.print(thermistor_value);
    Serial.print("F");
    Serial.print("\n");
    delay(1000);
    // Write to ThingSpeak
    // pieces of information in a channel
    int x = ThingSpeak.readFloatField(myChannelNumber,3, myReadAPIKey); // check what data to writ

    switch (x)
    {
      case 1: //temp write
     {
       Serial.println("Channel read update successful./n"); 
       delay(15000);
       int y = ThingSpeak.writeField(myChannelNumber, 1, thermistor_value, myWriteAPIKey);
        
       if(y == 200) // check error code if failed
        {
          Serial.println("Channel write update successful.");
          delay(15000);
          int x = ThingSpeak.writeField(myChannelNumber, 3, 0, myWriteAPIKey);
        }//end of 1 field write
        
        else // if failed what happened??!?!?
        {
          Serial.println("Problem updating channel. HTTP error code " + String(y));
        }
        break;
     }
     
     case 2: //step count write
     {
       Serial.println("Channel read update successful./n");
       delay(15000);
       int steps = 12; // acceloremeter test number 
       int y = ThingSpeak.writeField(myChannelNumber, 2, steps, myWriteAPIKey);
        
       if(y == 200) // check error code if failed 
        {
          Serial.println("Channel write update successful.");
          delay(15000);
          int x = ThingSpeak.writeField(myChannelNumber, 3, 0, myWriteAPIKey);
        }//end of 2 field write 
        
        else // if failed what happened??!?!?
        {
          Serial.println("Problem updating channel. HTTP error code " + String(y));
        }
        break;
      }

      case 3://LED
      {
        Serial.println("Channel read update successful./n");
        int x = ThingSpeak.readFloatField(myChannelNumber,4, myReadAPIKey);
        
        int temp , a ;
char hex [ 32 ] ;
temp = 0 ;
while( x > 0 )
{
switch( x % 16 )
{
case 10 :
{
hex [ temp ] = 'A' ; break ;
}
case 11 :
{
hex [ temp ] = 'B'; break ;
}
case 12 :
{
hex [ temp ] = 'C' ; break ;
}
case 13 :
{
hex [ temp ] = 'D'; break ;
}
case 14 :
{
hex [ temp ] = 'E' ; break ;
}
case 15 :
{
hex [ temp ] = 'F'; break ;
}
default :
{
hex [ temp ] = ( x %16 ) + 0x30 ;
}
x = x / 16 ;
temp ++ ;
}
printf( " The Hexadecimal value of the decimal number is: " ) ;
for( a= ( temp-1 ) ; a >= 0 ; a--)
printf( "%c" , hex[ a ] ) ;


}
      }     
      default:
        {
          Serial.println("nothing to update.");
          break;
        }
      }//end read check then write
      
     lastTime = millis();
  }
}//end of loop
