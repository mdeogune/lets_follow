#include <QTRSensors.h>
#include <DRV8833.h>

#define KP 0.075
#define KD 0
#define KI 1/10000

#define normal_speed 220
#define max_speed 255
#define NUM_SENSORS   8   
#define TIMEOUT       2500  
#define EMITTER_PIN   2  
//
//// Create an instance of the DRV8833:
DRV8833 driver = DRV8833();
const int inputA1 = 5, inputA2 = 6, inputB1 = 10, inputB2 = 11;
QTRSensorsRC qtrrc((unsigned char[]) {A0,A1,A2,A3,A4,A5,3,2},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensors[NUM_SENSORS];
unsigned int position=0;
unsigned int lastpos=0;
int error,motorspeed,lms,rms,lasterror;

void setup() {
Serial.begin(9600);
 
  
  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();     delay(20);
     
  }

  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
}

void loop() {
  // put your main code here, to run repeatedly:
    position = qtrrc.readLine(sensors);
  //Serial.println(position);
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensors[i]);
    Serial.print('\t');
  }Serial.print('\n');
   


   int left=0,right=0,flag=0,straight=0,left_angled=0,right_angled=0;

   if(sensors[1]<200 && sensors[6]<200 && sensors[3]<200 && sensors[4]<200)
{  
  uturn();

}

if(sensors[2]>200 && sensors[6] >200 )
{right=1;
 flag=1;
forward();
 delay(2);  //BRRODA VERIFY
 qtrrc.read(sensors);
 if(sensors[1]>200 && sensors[5]>200)
{left=1;  // define
}
}


if(sensors[1]>200 && sensors[5]>200)
{left=1;  // define
flag=1;
 forward();
 delay(2);  //BRRODA VERIFY
 qtrrc.read(sensors);
 if(sensors[2]>200 && sensors[6
 ]>200)
{right=1;  // define
}
}

if(flag==1)
{ 
 while(1)
  {
  qtrrc.readLine(sensors);
  forward();
  if(sensors[4]<200 && sensors[3]<200)
  break;
  if(sensors[0]<200 && sensors[5]<200)
  break;
  }
 // delay(320);
 qtrrc.readLine(sensors);
  delay(5);  //faltu extra
 // impulse_stop();
 /*
if(sensors[5]>200)
{left_angled=1;
left=0;*/

}

if (sensors[0]<200 && sensors[1]<200 && sensors[2]<200 &&sensors[3]<200 && sensors[4]<200 && sensors[5]<200 && sensors[7]<200 &&sensors[6]<200)
{
}
else
{straight=1;

}

if(left==1)
{leftturn();
}

else if(straight==1)
{  
}
else if(right==1)
{rightturn();
}



 follow_segment();



}

void follow_segment(){


  error = position - 3500;
//  integral+=error;
//+integral * KI
  motorspeed = KP * error + KD * (error - lasterror);
  lasterror = error;
//lastpos=position;

 lms= normal_speed+motorspeed;
  rms= normal_speed-motorspeed;

  if(lms>max_speed)
  lms=max_speed;
  if(lms<0)
  lms=0;
  if(rms>max_speed)
  rms=max_speed;
  if(rms<0)
  rms=0;
  

  driver.motorBForward(lms);
  driver.motorAForward(rms);

       
}


void leftturn()
{
  driver.motorBReverse(200);
  driver.motorAForward(200);

 // delay(300);    //DOUBLE CHECK BRODA
  qtrrc.readLine(sensors);
  while(sensors[2]<200)
  {qtrrc.readLine(sensors);
  }
  }

void rightturn()
{
  driver.motorBForward(200);
  driver.motorAReverse(200);
  //delay(300);    //DOUBLE CHECK BRODA
  qtrrc.readLine(sensors);
  while(sensors[5]<200)
  {qtrrc.readLine(sensors);
  }
  }

   void forward()
  {  driver.motorBForward(150);
  driver.motorAForward(150);
  }

   void uturn()
  {
  driver.motorBForward(200);
  driver.motorAReverse(200);
  
  qtrrc.readLine(sensors);
  while(sensors[5]<200)
  {qtrrc.readLine(sensors);
  }
    }
  
