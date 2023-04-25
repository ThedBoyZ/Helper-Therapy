zzzz9o/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
// ถ้ากดปุ่ม S1 จะเขียนสถานะ ON และสถานะเป็น 1-4 และสุ่มตัวเลข 0-19 ไปยังตัวแปร tmpdata1 และ tmpdata2 ไปส่งข้อมูลยัง php ทุกๆ 5 วินาที
// ถ้ากดปุ่ม S2 จะเขียนสถานะ OFF เพื่อยกเลิกโหมดการทำงาน 1-4 เป็น 0
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
const char* ssid     = "Platong 2.4G";    // 
const char* password = "Beer0838856317";    // 

const char* host = "student.sangsiri.net";
//const char* streamId   = "....................";
//const char* privateKey = "....................";
int nub1=0,ideal=0;
int nub2=0;
int nub3=0;
int nub4=0;
int c=0,c2=0;
int ran1=0;
int Pulse = 35;
int Signal;  
int Threshold = 2410;
int bpm=0;    
int check;
//** Button ปุ่มกด **//
int iStart=1;
long start=0;
int iClear=0;
int swOn=0;
int s1=16;
int s2=14;
//** Mini Pi & HCSR04**//
int buzzer = 13 ;
int MiniPi = 34;
const int pingPin = 18; //18 Trig
int inPin = 19; //19 ECHO
const int httpPort = 80;
char M1[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  matrix.begin(0x70);
  matrix.setRotation(1);
  static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100 };
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(32,INPUT_PULLUP);
  pinMode(33,INPUT_PULLUP);
  pinMode(s1,INPUT_PULLUP);
  pinMode(s2,INPUT_PULLUP);
  pinMode(MiniPi, INPUT);
  pinMode (buzzer, OUTPUT) ;
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");}
  Serial.println("");
  Serial.println("WiFi connected");
  //****** Thepary Updates = 0 ******//
     start = millis();
     Serial.print("connecting to ");
     Serial.println(host); 
     // Use WiFiClient class to create TCP connections
     WiFiClient client;
     if (!client.connect(host, httpPort)) {
       Serial.println("connection failed");
       return;} 
        
     // We now create a URI for the request to   
     String url = "/arduino/therapy/therapy_update.php?";  /////    
     url += "gait=0";                       //Thepary update = 0
     url += "&limb=0";                             
     url += "&vision=0";
     url += "&coordinator=0";

    Serial.print("Requesting URL: ");
    Serial.println(url);
      
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
     }    
      // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    }    
  Serial.println();
  Serial.println("closing connection");
  matrix.clear();
  matrix.drawBitmap(4, 0, smile_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(4000);
  matrix.clear();
  matrix.writeDisplay();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  for (int8_t x=7; x>=-80; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("Gait Mode");
    matrix.writeDisplay();
    delay(70);
  }
}
void loop()
{
  Mode();
  if(iStart==1) { 
    Serial.println(nub1);
    Action_Ultra();
    Action_Mini(); }
  else if(iStart==2){
    Serial.println(nub2);
    Action_Ultra();
    Action_Mini(); }
  else if(iStart==3) {
    Vision();
    Serial.print("Nub3 = ");  
    Serial.println(nub3);
    delay(2500);
  }
  else if(iStart==4){
  Coordinator();
  delay(2000);
  Serial.print("Nub4 = ");  
  Serial.println(nub4);
  Serial.print("♥ BPM = ");  
  Serial.println(bpm);
  delay(3000);    
  }
 Mode();
 sentdata();
}
 void sentdata(){
//    delay(10000);
   if ((millis() - start > 60000) && (iClear==0)) // 5000 =5 Sec
    {    
      start = millis();
      Serial.print("connecting to ");
      Serial.println(host);
      
      WiFiClient client;      
      // Use WiFiClient class to create TCP connections
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;} 
        
      // We now create a URI for the request to   
       String url = "/arduino/therapy/therapy_update.php?";  /////    
      url += "gait=";                              /////
      url += nub1;                             //Thepary update
      url += "&limb=";                             
      url += nub2;
      url += "&vision=";
      url += nub3;
      url += "&coordinator=";
      url += nub4;

      Serial.print("Requesting URL: ");
      Serial.println(url);
      
      // This will send the request to the server
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }    
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()) {
    String line = client.readStringUntil('\r');
        Serial.print(line);
      }    
      Serial.println();
      Serial.println("closing connection");
      
      //**************************************  
      
      // Use WiFiClient class to create TCP connections
      delay(2000);
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;}
               
      if(iStart==1){
       double duration, cm;
       pinMode(pingPin, OUTPUT);
       digitalWrite(pingPin, LOW);
       delayMicroseconds(2);
       digitalWrite(pingPin, HIGH);
       delayMicroseconds(5);
       digitalWrite(pingPin, LOW);
       pinMode(inPin, INPUT);
       duration = pulseIn(inPin, HIGH);
       cm = microsecondsToCentimeters(duration);
       if(nub1!=ideal){
       String url2 = "/arduino/therapy/gait_mode.php?";    
              url2 += "d1=";                            
              url2 += cm;                
              url2 += "&d2=";                            
              url2 += nub1;     
      Serial.print("Requesting URL: ");
      Serial.println(url2);    
      // This will send the request to the server
      client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n");
      timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }    
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()) {
    String line = client.readStringUntil('\r');
        Serial.print(line);
      }    
      Serial.println();
      Serial.println("closing connection");
       }
       ideal = nub1;              
    }
      else if(iStart==2){
       double duration, cm;
       pinMode(pingPin, OUTPUT);
       digitalWrite(pingPin, LOW);
       delayMicroseconds(2);
       digitalWrite(pingPin, HIGH);
       delayMicroseconds(5);
       digitalWrite(pingPin, LOW);
       pinMode(inPin, INPUT);
       duration = pulseIn(inPin, HIGH);
       cm = microsecondsToCentimeters(duration);
       if(nub2!=ideal){
       String url2 = "/arduino/therapy/limb_mode.php?";    
              url2 += "d1=";                            
              url2 += cm;                
              url2 += "&d2=";                            
              url2 += nub2;
      Serial.print("Requesting URL: ");
      Serial.println(url2);    
      // This will send the request to the server
      client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n");
      timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }    
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()) {
    String line = client.readStringUntil('\r');
        Serial.print(line);
      }    
      Serial.println();
      Serial.println("closing connection");
      }
      ideal = nub2;                              
     }
      else if(iStart==3){
       if(nub3!=ideal){
       String url2 = "/arduino/therapy/vision_mode.php?";    
              url2 += "d1=";                            
              url2+= M1[ran1];                
              url2 += "&d2=";                            
              url2 += nub3;
      Serial.print("Requesting URL: ");
      Serial.println(url2);    
      // This will send the request to the server
      client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n");
      timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }    
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()) {
    String line = client.readStringUntil('\r');
        Serial.print(line);
      }    
      Serial.println();
      Serial.println("closing connection");
       }
       ideal = nub3;                                                
      }
      else if(iStart==4){
       if(nub4!=ideal){
       String url2 = "/arduino/therapy/coordinator_mode.php?";    
              url2 += "d1=";                            
              url2 += bpm;                
              url2 += "&d2=";                            
              url2 += nub4; 
      Serial.print("Requesting URL: ");
      Serial.println(url2);    
      // This will send the request to the server
      client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n");
      timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }    
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()) {
    String line = client.readStringUntil('\r');
        Serial.print(line);
      }    
      Serial.println();
      Serial.println("closing connection");                                                             
      }
      ideal = nub4;
  }}}
void Mode(){
    if(digitalRead(s1)==0)
  {
    while(digitalRead(s1)==0) delay(5);
    swOn=1;
    iClear=0;
    if(iStart==0)
      iStart=1;
    else if(iStart==1){
      iStart=2;
      matrix.setTextSize(1);
         for (int8_t x=7; x>=-80; x--) {
          matrix.clear();
          matrix.setCursor(x,0);
          matrix.print("Limb Mode");
          matrix.writeDisplay();
          delay(70);
        }
       ideal = 0;
     }        
    else if(iStart==2){
      iStart=3;
      matrix.setTextSize(1);
       for (int8_t x=7; x>=-80; x--) {
         matrix.clear();
         matrix.setCursor(x,0);
         matrix.print("Vision Mode");
         matrix.writeDisplay();
         delay(55);
        }
      ideal = 0;
    }        
    else if(iStart==3){
      iStart=4;
      matrix.setTextSize(1); 
      matrix.setRotation(1);    
       for (int8_t x=7; x>=-100; x--) {
         matrix.clear();
         matrix.setCursor(x,0);
         matrix.print("Coordinator Mode");
         matrix.writeDisplay();
         delay(45);
        }
      ideal = 0;    
    }    
    else if(iStart==4){
      iStart=1;
      matrix.setTextSize(1);
        for (int8_t x=7; x>=-80; x--) {
          matrix.clear();
          matrix.setCursor(x,0);
          matrix.print("Gait Mode");
          matrix.writeDisplay();
          delay(60);
        }
      ideal = 0;        
    }        
  }
  else if(digitalRead(s2)==0)
  {
    while(digitalRead(s2)==0) delay(10);
    swOn=0;       
    iStart=0;
    iClear=1;
  }
  Serial.print("ON=");
  Serial.println(swOn);   // turn the LED on (HIGH is the voltage level)  
  Serial.print("MODE=");
  Serial.println(iStart);   // turn the LED on (HIGH is the voltage level)  
  delay(100);
}
void Action_Mini(){
  int value = digitalRead(MiniPi);
  if(value==HIGH){
    Serial.println("Motion Detect");
     if(iStart==2){
      digitalWrite(27,HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (200) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (50) ;
         nub2++;
      digitalWrite(27,LOW);
    }
   }         
  else if(value==LOW)
    Serial.println("Motion Not Detect");     
  delay(2000);
 }
void Vision(){
  ran1 = rand() % 26; /* random int between 0 and 19 */ 
  int ran2 = rand() % 2; /* random Rotation between 0 and 1 */
  c=0;
  matrix.clear();
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setCursor(6,0);
   if(ran2==0 || ran1==7 || ran1==8 || ran1==9 || ran1==13 || ran1==14 || ran1==19 || ran1==21 || ran1==22 || ran1==23 || ran1==24){
     Serial.println(ran1);
     Serial.print("Random1 = ");
     Serial.print(ran2);
     matrix.setRotation(1);
     matrix.print(M1[ran1]);
     matrix.writeDisplay();
    if(digitalRead(32)==0 && c==0){
    unsigned char i ;
    for (i = 0; i <20; i++) // When a frequency sound
   {
  digitalWrite (buzzer, HIGH) ; //send tone
  delay (200) ;
  digitalWrite (buzzer, LOW) ; //no tone
  delay (100) ;
  }
  nub3++;
  c=1;
  }
 else {
 delay(1500);
 if(digitalRead(32)==0 && c==0){
 unsigned char i ;
 for (i = 0; i <20; i++) // When a frequency sound
 {
 digitalWrite (buzzer, HIGH) ; //send tone
 delay (200) ;
 digitalWrite (buzzer, LOW) ; //no tone
 delay (100) ;
 }
 nub3++;
 c=1;
 }
 else {
 delay(1500); 
 if(digitalRead(32)==0 && c==0){
  unsigned char i ;
  for (i = 0; i <20; i++) // When a frequency sound
  {
  digitalWrite (buzzer, HIGH) ; //send tone
  delay (200) ;
  digitalWrite (buzzer, LOW) ; //no tone
  delay (100) ;
  }
   nub3++;
   c=1;
   }
  else{
   delay(1500); 
   if(digitalRead(32)==0 && c==0){
   unsigned char i ;
  for (i = 0; i <20; i++) // When a frequency sound
  {
   digitalWrite (buzzer, HIGH) ; //send tone
   delay (200) ;
   digitalWrite (buzzer, LOW) ; //no tone
   delay (100) ;
   }
  nub3++;
  c=1;
   }
   else{
    delay(1500); 
   if(digitalRead(32)==0 && c==0){
    unsigned char i ;
    for (i = 0; i <20; i++) // When a frequency sound
   {
   digitalWrite (buzzer, HIGH) ; //send tone
   delay (200) ;
   digitalWrite (buzzer, LOW) ; //no tone
   delay (100) ;
   }
  nub3++;
  } }} }}}
   else if(ran2==1 && ran1!=7 && ran1!=8 && ran1!=9 && ran1!=13 && ran1!=14 && ran1!=19 && ran1!=21 && ran1!= 22 && ran1!=23 && ran1!=24){
     Serial.println(ran1);
     Serial.print("Random2 = ");
     Serial.print(ran2);
     matrix.setRotation(-1);
     matrix.print(M1[ran1]);
     matrix.writeDisplay();
    if(digitalRead(33)==0 && c==0){
    unsigned char i ;
    for (i = 0; i <20; i++) // When a frequency sound
   {
  digitalWrite (buzzer, HIGH) ; //send tone
  delay (200) ;
  digitalWrite (buzzer, LOW) ; //no tone
  delay (100) ;
  }
  nub3++;
  c=1;}
 else {
 delay(1500);
 if(digitalRead(33)==0 && c==0){
 unsigned char i ;
 for (i = 0; i <20; i++){
 digitalWrite (buzzer, HIGH) ; //send tone
 delay (200) ;
 digitalWrite (buzzer, LOW) ; //no tone
 delay (100) ;}
 nub3++;
 c=1;}
 else {
 delay(1500); 
 if(digitalRead(33)==0 && c==0){
  unsigned char i ;
  for (i = 0; i <20; i++){
  digitalWrite (buzzer, HIGH) ; //send tone
  delay (200) ;
  digitalWrite (buzzer, LOW) ; //no tone
  delay (100) ;}
   nub3++;
   c=1;}
  else{
   delay(1500); 
   if(digitalRead(33)==0 && c==0){
   unsigned char i ;
  for (i = 0; i <20; i++){
   digitalWrite (buzzer, HIGH) ; //send tone
   delay (200) ;
   digitalWrite (buzzer, LOW) ; //no tone
   delay (100) ;}
  nub3++;
  c=1;}
   else{
    delay(1500); 
   if(digitalRead(33)==0 && c==0){
    unsigned char i ;
    for (i = 0; i <20; i++){
   digitalWrite (buzzer, HIGH) ; //send tone
   delay (200) ;
   digitalWrite (buzzer, LOW) ; //no tone
   delay (100) ;}
  nub3++;
}}}}}}
  Serial.println(" ");  
  delay(1000);   }
double microsecondsToCentimeters(double microseconds)
{
  // ความเร็วเสียงในอากาศประมาณ 340 เมตร/วินาที หรือ 29 ไมโครวินาที/เซนติเมตร
  // ระยะทางที่ส่งเสียงออกไปจนเสียงสะท้อนกลับมาสามารถใช้หาระยะทางของวัตถุได้
  // เวลาที่ใช้คือ ระยะทางไปกลับ ดังนั้นระยะทางคือ ครึ่งหนึ่งของที่วัดได้
  return microseconds / 29 / 2;
}  
void Action_Ultra(){
  double duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.println("cm");
  matrix.clear();
  matrix.setTextSize(0.2);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setCursor(0,0);
  matrix.print(cm);
  matrix.writeDisplay();
  if(cm <=100 && iStart==1){
     digitalWrite(26,HIGH);   // turn the LED on (HIGH is the voltage level)
     unsigned char i ;
   for (i = 0; i <10; i++) // When a frequency sound
    {
     digitalWrite (buzzer, HIGH) ; //send tone
     delay (100) ;
     digitalWrite (buzzer, LOW) ; //no tone
     delay (100) ;
    }
     nub1++;
     digitalWrite(26,LOW);}
  else if(cm <=70 && iStart==2 && digitalRead(MiniPi)==LOW){
     digitalWrite(27,HIGH);   // turn the LED on (HIGH is the voltage level)
     unsigned char i ;
   for (i = 0; i <10; i++){ // When a frequency sound{
     digitalWrite (buzzer, HIGH) ; //send tone
     delay (200) ;
     digitalWrite (buzzer, LOW) ; //no tone
     delay (50) ;
     }
     nub2++;
  digitalWrite(27,LOW);   // turn the LED on (HIGH is the voltage level)
  delay(10);}
}
void Coordinator(){
   int ran3 = rand() % 3; /* random int between 0 and 19 */
   Serial.print("Coor = ");
   Serial.println(ran3+1);
   digitalWrite(26,LOW);
   digitalWrite(27,LOW);  
   matrix.clear(); 
   matrix.writeDisplay(); 
  if(ran3==0){
     bpm=0;
     c2=0;
     digitalWrite(26,HIGH);
     if(analogRead(Pulse) > Threshold && c2==0){ // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
        Signal = analogRead(Pulse);
        Serial.print(Signal);
        Serial.println(" ");
        digitalWrite(LED_BUILTIN,LOW);
         unsigned char i ;
      for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (100) ;
        }
        bpm = (12*Signal/550);
        matrix.setCursor(0,0);
        matrix.print(bpm);
        matrix.writeDisplay();
        nub4++;
        c2=1;
        digitalWrite(LED_BUILTIN,HIGH);    
        } 
      else {
        delay(1500);
         if(analogRead(Pulse) > Threshold && c2==0){ 
         Signal = analogRead(Pulse);
         Serial.print(Signal);
         Serial.println(" ");
         digitalWrite(LED_BUILTIN,LOW);
         unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (100) ;
        }
        bpm = (12*Signal/550);
        matrix.setCursor(0,0);
        matrix.print(bpm);
        matrix.writeDisplay();
        nub4++;
        c2=1;
        digitalWrite(LED_BUILTIN,HIGH);    
        } 
       else {
        delay(1500);
         if(analogRead(Pulse) > Threshold && c2==0){
         Signal = analogRead(Pulse);
         Serial.print(Signal);
         Serial.println(" ");
         digitalWrite(LED_BUILTIN,LOW);
         unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (100) ;
        }
        bpm = (12*Signal/550);
        matrix.setCursor(0,0);
        matrix.print(bpm);
        matrix.writeDisplay();
        nub4++;
        c2=1;
        digitalWrite(LED_BUILTIN,HIGH);    
        }
       else {
        delay(1500);
         if(analogRead(Pulse) > Threshold && c2==0){
         Signal = analogRead(Pulse);
         Serial.print(Signal);
         Serial.println(" ");
         digitalWrite(LED_BUILTIN,LOW);
         unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (100) ;
        }
        bpm = (12*Signal/550);
        matrix.setCursor(0,0);
        matrix.print(bpm);
        matrix.writeDisplay();
        nub4++;
        c2=1;
        digitalWrite(LED_BUILTIN,HIGH);   
        }
       else {
        delay(1500);
         if(analogRead(Pulse) > Threshold && c2==0){
         Signal = analogRead(Pulse);
         Serial.print(Signal);
         Serial.println(" ");
         digitalWrite(LED_BUILTIN,LOW);
         unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (100) ;
        }
        bpm = (12*Signal/550);
        matrix.setCursor(0,0);
        matrix.print(bpm);
        matrix.writeDisplay();
        nub4++;
        digitalWrite(LED_BUILTIN,HIGH);    
        }
      else {
        delay(1500);
         if(analogRead(Pulse) > Threshold && c2==0){
         Signal = analogRead(Pulse);
         Serial.print(Signal);
         Serial.println(" ");
         digitalWrite(LED_BUILTIN,LOW);
         unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (100) ;
        }
        bpm = (12*Signal/550);
        matrix.setCursor(0,0);
        matrix.print(bpm);
        matrix.writeDisplay();
        nub4++;
        digitalWrite(LED_BUILTIN,HIGH);    
        }
     }    
   }   
  }
 }
}
}
  else if(ran3==1){
   int ran4 = rand() % 5;
   int ran5 = rand() % 2;
   Serial.print("flashing = ");
   Serial.print(ran4+1); 
   if(ran5==0){
   matrix.clear();
   matrix.setCursor(0,0);
   matrix.print("S-1");
   matrix.writeDisplay();
   }
   else if(ran5==1){
   matrix.clear();
   matrix.setCursor(0,0);
   matrix.print("S-2");
   matrix.writeDisplay();        
   }
   if(ran4==0){
    c2=0;
    delay(2000);
    digitalWrite(27,HIGH);
    delay(2000);
    digitalWrite(27,LOW);
      matrix.setTextSize(1); 
      matrix.setRotation(1);       
       for (int8_t x=7; x>=-90; x--) {
         matrix.clear();
         matrix.setCursor(x,0);
         matrix.print("Ready?");
         matrix.writeDisplay();
         delay(70);
        }    
      delay(2000);
      if(ran5==0){
       for(int x=1;x<3;x++){
         if((digitalRead(32)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==1){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   }
   else if(ran5==1){
       for(int x=1;x<3;x++){
         if((digitalRead(33)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==1){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   } 
  }
   else if(ran4==1){
    c2=0;
    for(int x=0;x<2;x++){
    delay(2000);   
    digitalWrite(27,HIGH);
    delay(2000);
    digitalWrite(27,LOW);
    }
      matrix.clear();
      matrix.setTextSize(1); 
      matrix.setRotation(1);       
       for (int8_t x=7; x>=-90; x--) {
         matrix.clear();
         matrix.setCursor(x,0);
         matrix.print("Ready?");
         matrix.writeDisplay();
         delay(70);
       }
      delay(2000);
      if(ran5==0){
       for(int x=1;x<5;x++){
         if((digitalRead(32)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==2){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   }
   else if(ran5==1){
       for(int x=1;x<5;x++){
         if((digitalRead(33)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==2){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   }  
  }   
   else if(ran4==2){
    for(int x=0;x<3;x++){
    delay(2000);  
    digitalWrite(27,HIGH);
    delay(2000);
    digitalWrite(27,LOW);  
    }
      matrix.clear();
      matrix.setTextSize(1); 
      matrix.setRotation(1);       
       for (int8_t x=7; x>=-90; x--) {
         matrix.clear();
         matrix.setCursor(x,0);
         matrix.print("Ready?");
         matrix.writeDisplay();
         delay(70);
       }
      delay(2000);
      if(ran5==0){
       for(int x=1;x<6;x++){
         if((digitalRead(32)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==3){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   }
   else if(ran5==1){
       for(int x=1;x<6;x++){
         if((digitalRead(33)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==3){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   }  
  }      
   else if(ran4==3){
    for(int x=0;x<4;x++){
    delay(2000);  
    digitalWrite(27,HIGH);
    delay(2000);
    digitalWrite(27,LOW);       
    }
      matrix.clear();
      matrix.setTextSize(1); 
      matrix.setRotation(1);       
       for (int8_t x=7; x>=-90; x--) {
         matrix.clear();
         matrix.setCursor(x,0);
         matrix.print("Ready?");
         matrix.writeDisplay();
         delay(70);
       }
      delay(2000);
      if(ran5==0){
       for(int x=1;x<8;x++){
         if((digitalRead(32)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==4){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   }
   else if(ran5==1){
       for(int x=1;x<8;x++){
         if((digitalRead(33)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==4){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   } 
  }
   else if(ran4==4){   
    for(int x=0;x<5;x++){
    delay(2000);
    digitalWrite(27,HIGH);
    delay(2000);
    digitalWrite(27,LOW);        
   }
      matrix.clear();
      matrix.setTextSize(1); 
      matrix.setRotation(1);       
       for (int8_t x=7; x>=-90; x--) {
         matrix.clear();
         matrix.setCursor(x,0);
         matrix.print("Ready?");
         matrix.writeDisplay();
         delay(70);
       }
      delay(2000);
      if(ran5==0){
       for(int x=1;x<10;x++){
         if((digitalRead(32)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==5){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      }
   }
   else if(ran5==1){
       for(int x=1;x<10;x++){
         if((digitalRead(33)==0)){
       unsigned char i ;
      for (i = 0; i <7; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2++;
      delay(1500); 
      }
      }
       if(c2==5){
       unsigned char i ;
        for (i = 0; i <20; i++) // When a frequency sound
        {
        digitalWrite (buzzer, HIGH) ; //send tone
        delay (150) ;
        digitalWrite (buzzer, LOW) ; //no tone
        delay (125) ;
       }
       nub4++; 
      } } 
 }
   Serial.println(" ");
} 
  else if(ran3==2){
  digitalWrite(26,HIGH); 
  digitalWrite(27,HIGH);
  c2=0;
  delay(2000);
  matrix.clear();
  matrix.setCursor(0,0);
  matrix.print(nub4);
  matrix.writeDisplay();
     if((digitalRead(32)==0) && (digitalRead(33)==0)){
       unsigned char i ;
      for (i = 0; i <25; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
     c2=1;
     nub4++;
     matrix.clear();
     matrix.setCursor(0,0);
     matrix.print(nub4);
     matrix.writeDisplay(); 
    }
    else{
     delay(1500);
     if(digitalRead(32)==0 && digitalRead(33)==0 && c2==0){ 
       unsigned char i ;
      for (i = 0; i <25; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2=1;
      nub4++;
      matrix.clear();
      matrix.setCursor(0,0);
      matrix.print(nub4);
      matrix.writeDisplay(); 
    }
     else{
     delay(1500);
     if(digitalRead(32)==0 && digitalRead(33)==0 && c2==0){ // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
       unsigned char i ;
      for (i = 0; i <25; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2=1;
      nub4++;
      matrix.clear();
      matrix.setCursor(0,0);
      matrix.print(nub4);
      matrix.writeDisplay(); 
    }
     else{
     delay(1500);
     if(digitalRead(32)==0 && digitalRead(33)==0 && c2==0){ // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
       unsigned char i ;
      for (i = 0; i <25; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;
      }
      c2=1;
      nub4++;
      matrix.clear();
      matrix.setCursor(0,0);
      matrix.print(nub4);
      matrix.writeDisplay(); 
    }
      else{
     delay(1500);
     if(digitalRead(32)==0 && digitalRead(33)==0 && c2==0){ // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
       unsigned char i ;
      for (i = 0; i <25; i++) // When a frequency sound
      {
      digitalWrite (buzzer, HIGH) ; //send tone
      delay (125) ;
      digitalWrite (buzzer, LOW) ; //no tone
      delay (125) ;}
      c2=1;
      nub4++;
      matrix.clear();
      matrix.setCursor(0,0);
      matrix.print(nub4);
      matrix.writeDisplay(); 
    } }}}}
delay(1000);}}
