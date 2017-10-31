#include <QTRSensors.h>


#define lmtr1 A2
#define lmtr2 A3
#define rmtr1 A0
#define rmtr2 A1
#define lenable 10
#define renable 11
#define normal_speed 180
#define max_speed 255
//#define KP 0.075
#define KD 5
#define KI 1/10000
#define NUM_SENSORS   6     
#define TIMEOUT       2500  
#define EMITTER_PIN   2     
float KP=0.075;
long integral=0;
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 6, 7, 9, 12},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 

unsigned int count=0;
unsigned int sensors[6];
unsigned int position=0;
unsigned int lastpos=0;
int error,motorspeed,lms,rms,lasterror;

unsigned char found_straight=0;
unsigned char found_left=0;

unsigned char found_right=0;
unsigned char found_turn=0;
unsigned char Ll=0;
unsigned char Lr=0;
unsigned char Lf=0;


//----------------------------------------------------------------------------------------------------------------------------------

void setup() {

  
Serial.begin(9600);
pinMode(2, OUTPUT);
pinMode(13, OUTPUT);
digitalWrite(13, HIGH);   
  
  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();     delay(20);
     
  }
 
delay(2000);
digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
  

  for (int i = 0; i < NUM_SENSORS; i++)
  {
   // Serial.print(qtrrc.calibratedMinimumOn[i]);
    //Serial.print(' ');
  }
  for (int i = 0; i < NUM_SENSORS; i++)
  {
   // Serial.print(qtrrc.calibratedMaximumOn[i]);
   // Serial.print(' ');
  }

lasterror=0;
}



//--------------------------------------------------------------------------------------------------------------------------------------------------

void loop() { 
 // digitalWrite(2, HIGH); 
  position = qtrrc.readLine(sensors);
  //Serial.println(position);
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensors[i]);
    Serial.print('\t');
  }Serial.print('\n');

/////////////////////////bluetooth pid///////////////////////////
/*if (Serial.available()){
  float p;
  String read_;
  read_=Serial.readString();
  p=read_.toFloat();
  KP=p;
 // Serial.println(p);
}
///////////////////////////////////////////////////////////////
*/

  follow_segment();
 // hard_right();


   found_left=0;
   found_straight=0;
   found_right=0;
   found_turn=0;
  
  if(sensors[0] > 400&&sensors[1] > 400)
  
  { 
    found_left = 1;
  }
  if(sensors[4] > 400&&sensors[5] > 400)
    { 
      found_right = 1;
    }
  
 if (sensors[2]>200&&sensors[3]>200){
 
 found_straight = 1;}
 /* if ((sensors[0]<100&&sensors[1]<100&&sensors[2]<100&&sensors[3]<100&&sensors[4]<100&&sensors[5]<100)){
    hard_right();
        delay(500);
        stop_();}*/

if (found_right && found_straight ||found_left && found_straight){
  
  calibrate_turn();

 }

 
 


  Ll=found_left;
  Lf=found_straight;
  Lr=found_right;
  
}
void  calibrate_turn(){
  unsigned char next_found_left=0,next_found_right=0;
    if(sensors[0] > 400&&sensors[1] > 400)
  { 
    next_found_left = 1;
  }
  if(sensors[4] > 400&&sensors[5] > 400)
    { 
      next_found_right = 1;
    }
    move_forward();
    while((next_found_left==found_left) && (next_found_right==found_right)){
        position = qtrrc.readLine(sensors);
       
          next_found_right=0;
          next_found_left=0;
          
          if(sensors[0] > 400 && sensors[1] > 400)
           { 
            next_found_left = 1;
             Serial.print("new ");
        Serial.print(next_found_left);
        Serial.print(" old ");
        Serial.println(sensors[0]);
             }
          if(sensors[4] > 400&&sensors[5] > 400)
            { 
             next_found_right = 1;
            }
             if (sensors[0]<100 && sensors[1]>200||sensors[2]>200&&sensors[3]>200||sensors[4]>200 && sensors[5]<100){
 
              found_straight = 1;}
            
            
            move_forward();
            
            

      
    }
  move_forword();
  unsigned char dir = select_turn(found_left, found_straight, found_right,found_turn);
  turn(dir);

  
}



void straight()
{
  error = position - 2500;

  motorspeed = KP * error + KD * (error - lasterror);
  lasterror = error;

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
  
  analogWrite(lenable,lms);
  analogWrite(renable,rms);
  
  digitalWrite(lmtr1,HIGH);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,HIGH);
  digitalWrite(rmtr2,LOW);
  
}



void hard_right()
{  analogWrite(lenable,200);
  analogWrite(renable,200);
  digitalWrite(lmtr1,HIGH);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,LOW);
  digitalWrite(rmtr2,HIGH);
}

void soft_left()
{
   digitalWrite(lmtr1,LOW);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,HIGH);
  digitalWrite(rmtr2,LOW);
}

void soft_right()
{
   digitalWrite(lmtr1,HIGH);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,LOW);
  digitalWrite(rmtr2,LOW);
}
void hard_left()
{analogWrite(lenable,200);
  analogWrite(renable,200);
  digitalWrite(lmtr1,LOW);
  digitalWrite(lmtr2,HIGH);
  digitalWrite(rmtr1,HIGH);
  digitalWrite(rmtr2,LOW);
}
void turn_around()
{
  analogWrite(lenable,100);
  analogWrite(renable,100);
  digitalWrite(lmtr1,HIGH);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,LOW);
  digitalWrite(rmtr2,HIGH);
  delay(200);
    position = qtrrc.readLine(sensors);
if(sensors[3]>400&&sensors[2]>400){
    follow_segment();
  }
  else{
    turn_around();
  }
}
void move_forword()
{analogWrite(lenable,150);
  analogWrite(renable,150);
  digitalWrite(lmtr1,HIGH);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,HIGH);
  digitalWrite(rmtr2,LOW);
  delay(200);
   digitalWrite(lmtr1,LOW);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,LOW);
  digitalWrite(rmtr2,LOW);
  
}
void move_forward()
{analogWrite(lenable,100);
  analogWrite(renable,100);
  digitalWrite(lmtr1,HIGH);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,HIGH);
  digitalWrite(rmtr2,LOW);

  
}

void follow_segment(){
/*
 if ((sensors[0]<100&&sensors[1]<100&&sensors[2]<100&&sensors[3]<100&&sensors[4]<100&&sensors[5]<100)){
  turn_around();}*/
  error = position - 2500;
  integral+=error;
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
  
  analogWrite(lenable,lms);
  analogWrite(renable,rms);
 // Serial.println(lms-rms);
  digitalWrite(lmtr1,HIGH);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,HIGH);
  digitalWrite(rmtr2,LOW);

       
}

void stop_(){
  position = qtrrc.readLine(sensors);
  while(sensors[3]<400&&sensors[2]<400){
      position = qtrrc.readLine(sensors);
    //kuch mat kar be
  }
  if(sensors[3]>400&&sensors[2]>400){
    follow_segment();
  }
  else{
    stop_();
  }

  //follow_segment();
  
}
void turn(char dir)
{
    switch(dir)
    {
      case 'R':
        // Turn right.
        hard_right();
        delay(300);
        stop_();
        break;
    case 'L':
        // Turn left.
        hard_left();
        //set_motors(-100,100);
        delay(300);
         stop_();
        break;
   
    case 'B':
       // turn_around();
        hard_right();
        delay(500);
        stop_();
       // set_motors(100,-100);
      //  delay(100);
        break;
    case 'S':
   // follow_segment();
    
        // Don't do anything!
        break;
    }
}





char select_turn(unsigned char found_left, unsigned char found_straight,
  unsigned char found_right,unsigned char found_turn)
{
    // Make a decision about how to turn.  The following code
    // implements a left-hand-on-the-wall strategy, where we always
    // turn as far to the left as possible.
    
    
    if(found_left)
        return 'L';
    else if(found_straight)
        return 'S';
    else if(found_right)
        return 'R';
    else 
        return 'B';
}
