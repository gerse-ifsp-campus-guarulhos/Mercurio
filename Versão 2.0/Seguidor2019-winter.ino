
#include <QTRSensors.h>

#define kp 0.049
#define ki 0
#define kd 0.073
#define curv 7

 int mdir;
 int mesq;
 int p=0,i=0,d=0,pid;
 int lasterro = 0;
 int erro =0;
 int c=0;
 int marcdir=0;
 

// This example is designed for use with six analog QTR sensors. These
// reflectance sensors should be connected to analog pins A0 to A5. The
// sensors' emitter control pin (CTRL or LEDON) can optionally be connected to
// digital pin 2, or you can leave it disconnected and remove the call to
// setEmitterPin().
//
// The setup phase of this example calibrates the sensors for ten seconds and
// turns on the Arduino's LED (usually on pin 13) while calibration is going
// on. During this phase, you should expose each reflectance sensor to the
// lightest and darkest readings they will encounter. For example, if you are
// making a line follower, you should slide the sensors across the line during
// the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is. Improper calibration will result in
// poor readings.
//
// The main loop of the example reads the calibrated sensor values and uses
// them to estimate the position of a line. You can test this by taping a piece
// of 3/4" black electrical tape to a piece of white paper and sliding the
// sensor across it. It prints the sensor values to the serial monitor as
// numbers from 0 (maximum reflectance) to 1000 (minimum reflectance) followed
// by the estimated location of the line as a number from 0 to 5000. 1000 means
// the line is directly under sensor 1, 2000 means directly under sensor 2,
// etc. 0 means the line is directly under sensor 0 or was last seen by sensor
// 0 before being lost. 5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.

QTRSensors qtr;

const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

void setup()
{
  //Motores
  pinMode(6, OUTPUT); //Direito
  pinMode(2, OUTPUT);
  
  pinMode(5, OUTPUT); // Esquerdo
  pinMode(3, OUTPUT);

  //SensoreS
  pinMode(7, INPUT); //dIREITO
  pinMode(8, INPUT); // ESQUERDO
  
  // configure the sensors
  qtr.setTypeAnalog(  );
  qtr.setSensorPins((const uint8_t[]){A0, A1, A3, A4, A6, A7}, SensorCount);
  qtr.setEmitterPin(4);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
 /* for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
*/
   for(int i=0;i<6;i++){
    analogWrite(6, i);
    digitalWrite(LED_BUILTIN, HIGH);
       delay(200);
     digitalWrite(LED_BUILTIN, LOW);
       delay(200);
  }
   digitalWrite(2, 1);
   digitalWrite(3, 1);
  for(int i=0;i<165;i++){
    analogWrite(6, i);
    analogWrite(5, i);
       delay(3);
       
    }
    
}
  
void loop()
{ 
  uint16_t position = qtr.readLineWhite(sensorValues);


       marcdir = digitalRead(7);
   if(marcdir==0 ){c=c+1;delay(40);} else{delay(0);}
  //Serial.println(c);
   if(c>curv && marcdir==0){delay(20);while (1){digitalWrite(5,0); digitalWrite(6,0);analogWrite(2,0);analogWrite(3,0);}}

     else{
        erro = position -2500;
        
      if(erro < 800 && erro > (-800))
      {
          p =  erro * kp ;
          i= (erro + lasterro) * ki;
          d = (erro - lasterro) * kd;
  
          pid = p + i + d;

  
          mdir = 165 - pid;
          mesq = 165 + pid;
      }
      
      else{
     
    p =  erro * kp ;
    i= (erro + lasterro) * ki;
    d = (erro - lasterro) * kd;
  
    pid = p + i + d;

  
    mdir = 125 - pid;
    mesq = 125 + pid;
      }
  
  //Motor esquerdo A
    analogWrite(5, mesq);
    digitalWrite(3, 1);
    
  //Motor direito B
    analogWrite(6, mdir);
    digitalWrite(2, 1);
  lasterro = erro;

    //Serial.println(erro);
     /*Serial.println(pid);
    Serial.print("direito : ");
     Serial.println(mdir);
      Serial.print("esquerdo: ");
     Serial.println(mesq);
     Serial.println("  ");
     */ 
     }
}
