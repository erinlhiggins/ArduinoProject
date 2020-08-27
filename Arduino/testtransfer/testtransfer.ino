#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>
#include <LiquidCrystal.h>
SdFat sd;
SFEMP3Shield MP3player;
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);//12, 11, 5, 4, 3, 2
//int8_t current_track = 1;
const int buttonPin = 0;
const int buttonPin1 = 1;
const int buttonPin2 = 10;
const int buttonPin3 = 5;
int buttonState = 0;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int t;
double minh = 0;
double maxh = 0;
uint16_t playspeed;

#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2

//  Variables
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
//int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
boolean hasBeenSlow = false;
int threshold = 0;
int bpmPin = 11;
int buttonBPM = 0;
int b = 0;
int bcalc = 0; 
double calcStart = 0;
int bpm = 140;
int state = 0;
int totalHeartrate = 0;
int beats = 0;
int LED13 = 13;
int Threshold = 550;
int bpmt = 0;
int currentbeatt = 0;
int currentbeat = 0;
int timebetweent = 0;
int timebetween = 0;
int lastbeat = 0;
int lastbeatt = 0;
int startout = 0;
int needschange = 0;
int lastchange = 0;
int bpmeq = 0;
int curbeat = 0;
int timechange = 0;
unsigned long start = 0;
int lastButtonState = 0;
int8_t current_track = 0;
static int outputType = SERIAL_PLOTTER;
int byteRead;
int** beatData;
int count = 0;
boolean set = false;
boolean testing = false;
int playCount = 0;

// Arduino Beat Detector By Damian Peckett 2015
// License: Public Domain.

// Our Global Sample Rate, 5000hz
#define SAMPLEPERIODUS 200

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup() {
  pinMode(buttonState2, INPUT);
  pinMode(buttonState3, INPUT);
 pinMode(LED13, OUTPUT);
 //Serial.begin(9600);                      
lcd.begin(16, 2);
if(!sd.begin(9, SPI_HALF_SPEED)) sd.initErrorHalt();
if (!sd.chdir("/")) sd.errorHalt("sd.chdir");
MP3player.begin();
MP3player.setPlaySpeed(0);
 uint8_t result;
   Serial.begin(115200);
   //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");
  // put your setup code here, to run once:
  MP3player.setVolume(10,10);

  int count;
  result=0;
  count=0;
  do{
      ++count;
     result=MP3player.playTrack(count);
  }while(result==0);
  --count;
  beatData[2][count];
  for(int i=1;i<=count;i++){
    beatData[0][i]=i;
    beatData[1][i]=beatCounter(i);
  }
  sort(beatData[1],beatData[0],count);
}

void loop() {
  Serial.println("looping");
  int current = millis();
buttonState = digitalRead(buttonPin);
buttonState1 = digitalRead(buttonPin1);
buttonState2 = digitalRead(buttonPin2);
buttonState3 = digitalRead(buttonPin3);
if(buttonState3 == HIGH){
  needschange = millis()/1000;
  if((needschange-lastchange) > 0.1 && testing == true)
  {
    bpmt++;
  }
        lastchange = needschange;
        bpmeq=bpmt*10;
 }  
 if((current-start) >= 10000 && t==0 && testing==true){
  t=bpmeq;
  bpm = bpmeq;
  Serial.println("complete");
  MP3player.playMP3("Complete.mp3");
 }
if (buttonState2 == LOW) { 
Serial.println("Start");
MP3player.stopTrack();
MP3player.setPlaySpeed(0);
startout = 0;
if(state == 0){
  //threshold
  state = 1;
  if(t==0){
    Serial.println("calc thresh");
    MP3player.playMP3("Thresholdcalc.mp3");
    delay(10000);
    start = millis();
    MP3player.stopTrack();
    testing = true;
  }
  else{
    Serial.println("select new");
    MP3player.playMP3("Selectnew.mp3");
  }
}
else if(state == 1){
  //recovery
  Serial.println("recovery");
  Serial.print("Threshold:");
  Serial.println(t);
  state = 2;
  if(t == 0){
    t=170;
  }
  minh = t - (t*0.1) - 5;
  maxh = t - (t*0.1) + 5;
  Serial.print("min:");
  Serial.println(minh);
  Serial.print("max:");
  Serial.println(maxh);
  MP3player.playMP3("Recovery.mp3");
  delay(5000);
  MP3player.stopTrack();
  //MP3player.playMP3("170bpm.mp3");
}
else if(state == 2){
  //endurance
  Serial.println("endurance");
  Serial.print("Threshold:");
  Serial.println(t);
  state = 3;
  if(t == 0){
    t=170;
  }
  minh = t - 5;
  maxh = t + 5;
  Serial.print("min:");
  Serial.println(minh);
  Serial.print("max:");
  Serial.println(maxh);
  MP3player.playMP3("Endurance.mp3");
  delay(5000);
  MP3player.stopTrack();
  //MP3player.playMP3("170bpm.mp3");
}
else if(state == 3){
  //marathon
  Serial.println("marathon");
  state = 4;
  if(t == 0){
    t=170;
  }
  minh = t + (t*0.1) - 5;
  maxh = t + (t*0.1) + 5;
  Serial.print("min:");
  Serial.println(minh);
  Serial.print("max:");
  Serial.println(maxh);
  MP3player.playMP3("Marathon.mp3");
  delay(5000);
  MP3player.stopTrack();
 // MP3player.playMP3("170bpm.mp3");
}
else if(state == 4){
  //max capacity
  Serial.println("max");
  Serial.print("Threshold:");
  Serial.println(t);
  state = 0;
  if(t == 0){
    t=170;
  }
  minh = t + (t*0.2) - 5;
  maxh = t + (t*0.2) + 5;
  Serial.print("min:");
  Serial.println(minh);
  Serial.print("max:");
  Serial.println(maxh);
  MP3player.playMP3("Max.mp3");
  delay(5000);
  MP3player.stopTrack();
}

}
if(t!=0){
  MP3player.playTrack(beatData[0][0]);
  
}
if((current-start) >= 60000){
  bpm = bpmt/(current/60000);
}
       if(bpm < minh && t!=0 && minh != 0){
          MP3player.stopTrack();
          MP3player.playTrack(beatData[0][playCount++]);
        }
        if(bpm > maxh && t!=0 && maxh != 0){
          MP3player.stopTrack();
          MP3player.playTrack(beatData[0][playCount--]);
        }
else{ 

}
}

// 20 - 200hz Single Pole Bandpass IIR Filter
float bassFilter(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2]; 
    xv[2] = (sample) / 3.f; // change here to values close to 2, to adapt for stronger or weeker sources of line level audio  
    

    yv[0] = yv[1]; yv[1] = yv[2]; 
    yv[2] = (xv[2] - xv[0])
        + (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
    return yv[2];
}

// 10hz Single Pole Lowpass IIR Filter
float envelopeFilter(float sample) { //10hz low pass
    static float xv[2] = {0,0}, yv[2] = {0,0};
    xv[0] = xv[1]; 
    xv[1] = sample / 50.f;
    yv[0] = yv[1]; 
    yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
    return yv[1];
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
float beatFilter(float sample) {
    static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
    xv[0] = xv[1]; xv[1] = xv[2]; 
    xv[2] = sample / 2.7f;
    yv[0] = yv[1]; yv[1] = yv[2]; 
    yv[2] = (xv[2] - xv[0])
        + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
    return yv[2];
}

int beatCounter(int track){
  int count;
  char trackName[]= "track001.mp3";
  sprintf(trackName, "track%03d.mp3", track);
  MP3player.playMP3(trackName, 10000);
  uint32_t period = 60000;       // 1 minutes
  boolean samebeat;
  samebeat=false;
  unsigned long time = micros(); // Used to track rate
  float sample, value, envelope, beat, thresh;
  unsigned char i;
  for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
     

   
      // Read ADC and center so +-512
      sample = (float)analogRead(0)-503.f;

      // Filter only bass component
      value = bassFilter(sample);

      // Take signal amplitude and filter
      if(value < 0)value=-value;
      envelope = envelopeFilter(value);

      // Every 200 samples (25hz) filter the envelope 
      if(i == 200) {
              // Filter out repeating bass sounds 100 - 180bpm
              beat = beatFilter(envelope);

              // Threshold it based on potentiometer on AN1
              thresh = 0.02f * (float)analogRead(1);

              // If we are above threshold, light up LED
              if(beat > thresh){
                if(samebeat==false){
                  count++;
                  samebeat=true;
                }
              }else{
                if(samebeat==true){
                  count++;
                  samebeat=false;
                }
              }

              //Reset sample counter
              i = 0;
        }

        
    
    // Consume excess clock cycles, to keep at 5000 hz
        for(unsigned long up = time+SAMPLEPERIODUS; time > 20 && time < up; time = micros());
        //Serial.println(count);
        ++i;
  }
  MP3player.stopTrack();
  return count;
}

void sort(int a[], int b[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    int s = b[o];
                    a[o] = a[o+1];
                    b[o] = b[o+1];
                    a[o+1] = t;
                    b[o+1] = s;
                }
        }
    }
}


