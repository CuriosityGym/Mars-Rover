int ch1,ch2;
#include <MotorDriver.h>
MotorDriver m;
int preset=40;
int left,right;
void setup() {
  pinMode(A1, INPUT);
  pinMode(A0, INPUT); 
  Serial.begin(9600);
}

void loop() {
  ch2 = pulseIn(A1, HIGH);
  ch1 = pulseIn(A0, HIGH); 
  
  ch2= pwm(ch2);
  ch1=pwm(ch1);

  if(ch2<0)
  {
    ch1=-ch1;  
  }
  
  left=ch2+ch1;
  right=ch2-ch1;
  
  motor(left,right);
}

int pwm(int pulse){
  if(pulse>0){
      pulse = map(pulse, 1350, 1650,  -255, 255);
      pulse = constrain(pulse, -255, 255);
      }
    else
      pulse = 0;
  
    if(abs(pulse)< preset){
      pulse=0;
      }
   return pulse;
  }

void motor(int LeftMotor,int RightMotor){
  LeftMotor=constrain(LeftMotor,-255,255);
  RightMotor=constrain(RightMotor,-255,255);
  Serial.print(LeftMotor);
  Serial.print("\t");
  Serial.println(RightMotor);
  if(LeftMotor==0){
    m.motor(2,BRAKE,LeftMotor);
    }
    
  else if(LeftMotor>0){
    m.motor(2,FORWARD,LeftMotor);
    }
    
  else{
    m.motor(2,BACKWARD,abs(LeftMotor));
    }

  if(RightMotor==0){
    m.motor(3,BRAKE,RightMotor);
    }
  else if(RightMotor>0){
    m.motor(3,FORWARD,RightMotor);
    }
  else{
    m.motor(3,BACKWARD,abs(RightMotor));
    }
}
  
  
