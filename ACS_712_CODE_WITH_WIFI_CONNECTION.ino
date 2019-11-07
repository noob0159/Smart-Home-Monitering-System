#include <WiFi.h>
#include "time.h"

const int analogIn = 4;
const int sensorIn = 36;

float temp;
volatile int dayFlag = 0;
unsigned long int start_pressure, finished_pressure;
int value;
int store_temp=0;

int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module and 185 for 5A Module



#define AP_SSID "204-B"
#define AP_PSW  "204402key" 
unsigned long int start, finished, elapsed;
double Voltage = 0;
double VRMS = 0;
float AmpsRMS = 0;
int b=0,c=0;
int interval,finish,a = 0;

double AmpsRMS1 = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;

char date[50];

void setup(){ 
 Serial.begin(115200);
 pinMode (analogIn, INPUT_PULLUP);
 attachInterrupt (digitalPinToInterrupt (analogIn), runTime, CHANGE);
 WiFi.mode(WIFI_STA);
 WiFi.setSleep(false); 
 WiFi.begin(AP_SSID,AP_PSW);
 Serial.print("Wait for WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 delay(1000);
 printLocalTime();
 //state(VRMS,current1);

}

void loop(){
// b = AmpsRMS;
 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707;  //root 2 is 0.707
 AmpsRMS1 = (VRMS * 1000)/mVperAmp;
 AmpsRMS = AmpsRMS1-0.29-0.3;  // and there ,i think could be smaller ,that will be more stable when turn off

 
 //Serial.print(AmpsRMS,1);
// Serial.println(" Amps RMS");
 state(b,AmpsRMS); 
 if (dayFlag)
     {
          delay(1000);
          printLocalTime();
          Serial.print("Total elapsed sec:  ");
          Serial.println(elapsed/1000);
          Serial.println("");
          elapsed = 0;
          
     }
  dayFlag = 0;
  temperature();
}


float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here 
  int minValue = 4096;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 50) //sample 
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the minimum sensor value*/
           minValue = readValue;
       }
  
   }
 
    // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/4096.0;
   
 return result;
}

void state(int a, double b)
{
  if(a == 0 )
  {
      if(b >= 0.1 && c == 1) //how it can work i 
      {
        Serial.println("Bulb of TV room is on");
        printLocalTime();
        start = millis();
        Serial.println("\n");
        //Serial.println(interval);
        c = 0;
      }
  
      if( b <= -0.1 && c == 0)
      { 
        Serial.println("Bulb of TV room is off");
        printLocalTime();
        finished = millis();
        interval = finished - start;
        a = 1;
      }
      
  }

  if(a == 1)
  {
        Serial.print("time interval: ");
        Serial.print(interval/1000);
        Serial.println("s");
        Serial.println("\n");
        c = 1;
  }
  a = 0;
}


void printLocalTime()
{
//  struct tm timeinfo;
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  static const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};
  if(!localtime(&t)){
    Serial.println("Failed to obtain time");
    return;
  }
 // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.printf("%04d/%02d/%02d(%s) %02d:%02d:%02d\n",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        wd[tm->tm_wday],
        tm->tm_hour, tm->tm_min, tm->tm_sec);
 sprintf(date,"%04d/%02d/%02d(%s) %02d:%02d:%02d\n",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        wd[tm->tm_wday],
        tm->tm_hour, tm->tm_min, tm->tm_sec);
 //Serial.print(date);
}

// for pressure sensor
void runTime()
{

     if (dayFlag == 0)
     {
          value = digitalRead (analogIn);
         

          if (value == 1)
          {
               Serial.println("person is sitting on chair in TV  room ");
               start_pressure = millis();
               dayFlag = 0;
          }

          if (value == 0)
          {
               Serial.print("");
               Serial.println("case left the TV room chair");
               finished_pressure = millis();
               elapsed = finished_pressure - start_pressure;
               dayFlag = 1;
          }
     }
}

void temperature()
{
    temp = analogRead(39);
    temp = temp * 0.12207031; 
   
    if( ((store_temp - temp) > 1) || ((temp-store_temp) > 1))   
      {
        store_temp = temp;
        Serial.print("Temperture: ");
        Serial.print(temp,0);
        Serial.print("*C");
        Serial.println();
        printLocalTime();        
      }
  
}
