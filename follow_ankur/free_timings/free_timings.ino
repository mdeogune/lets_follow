#include <QTRSensors.h>
#define LED1 2
#define LED2 13
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
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 6, 7, 9, 12},NUM_SENSORS, TIMEOUT, EMITTER_PIN); // sensors 0 through 7 are connected to digital pins 3 through 10, respectivel

void (*action)() ;
class Bot{
  private:
    unsigned int sensors[6];z   
    unsigned int pos=0;
    unsigned int lastpos=0;
    int lasterror=0;
    unsigned char Lf,Ll,Lr=0; 
  public:
    enum direction{Left,Right,Forward,TurnAround}dir; 
    void read_sensor();
    void follow_segment();
  
};


void Bot::follow_segment(){
  int error,motorspeed,lms,rms,node;  
  bot.read_sensors()
  if(node=bot.has_node())
  {
    graph.register_node(node);
    bot.dir=graph.get_direction();
    action=bot.turn;
    return
  }
  error = pos - 2500; 
  motorspeed = KP * error + KD * (error - lasterror);
  lasterror = error; 
 `lms= normal_speed+motorspeed;
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

void Bot::turn()
{
    switch(dir)
    {
      case Right: 
        hard_right();
        break;
    case Left: 
        hard_left();
        break; 
    case TurnAround:
        turn_around();
        break;
    }
}

bool Sensor(int x) 
{
  return bot.sensors[x]<200;
}


void Bot:: execute(){}
void Bot:: read_sensor(){
  pos = qtrrc.readLine(sensors); //Serial.println(pos);
}


int command;
class Graph
{
  public:
  int check_conditions(Bot);
};
Bot bot;
//----------------------------------------------------------------------------------------------------------------------------------

void setup() {
    
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(13, HIGH);   
  for (int i = 0; i < 100; i++)  
  {
    qtrrc.calibrate();     delay(20);
  }
  delay(2000);
  action=bot.follow_segment;
  
}


//--------------------------------------------------------------------------------------------------------------------------------------------------
Graph graph;
void loop() {
  action();
}

void Bot::check_conditions()
{
    
  if(Sensor(0)&&Sensor(1))
  { 
    found_left = 1;
  }
  if(Sensor(4)&&Sensor(5))
  { 
    found_right = 1;
  }
  if (Sensor(1)||Sensor(2)&&Sensor(3)||Sensor(4))
  {
    found_straight = 1;
  }

}

void Bot::straight()
{
  int error,motorspeed,lms,rms;  
  error = pos - 2500;

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
{  
  analogWrite(lenable,130);
  analogWrite(renable,130);
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
{analogWrite(lenable,130);
  analogWrite(renable,130);
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
  if (Sensor(2) ||Sensor(3)||Sensor(4)||Sensor(5)||Sensor(0)||Sensor(1)){
    digitalWrite(lmtr1,LOW);
  digitalWrite(lmtr2,LOW);
  digitalWrite(rmtr1,LOW);
  digitalWrite(rmtr2,LOW);
  delay(200); 
  }
  
}


