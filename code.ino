#include <MotorDriver.h>
//#include <Servo.h>
//Servo myservo;  
//int pos = 0;  
MotorDriver m;
int preset=70;
int left,right;
int ch1,ch2; 

void setup() {
  pinMode(A1, INPUT);
  pinMode(A0, INPUT); 
//  pinMode(A5, INPUT);
  Serial.begin(9600);
//  myservo.attach(9);
//  myservo.write(90); 
}

void loop() {
  //ch6 = pulseIn(A5, HIGH);
  ch2 = pulseIn(A1, HIGH);
  ch1 = pulseIn(A0, HIGH); 

  //ch6=servo(ch6);
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
      pulse = map(pulse, 1325, 1650,  -255, 255);
      pulse = constrain(pulse, -255, 255);
      }
    else
      pulse = 0;
  
    if(abs(pulse)< preset){
      pulse=0;
      }
   return pulse;
  }

//int servo(int pulse){
//  if(pulse>0){
//      pulse = map(pulse, 1980, 990, 0, 180);
//      pulse = constrain(pulse, 10, 170);
//      }
//    else
//      pulse = 0;
//  
//    /*if(abs(pulse)< preset){
//      pulse=0;
//      }*/
//   return pulse;
//  }

 
void motor(int LeftMotor,int RightMotor){
  LeftMotor=constrain(LeftMotor,-255,255);
  RightMotor=constrain(RightMotor,-255,255);
  Serial.print(LeftMotor);
  Serial.print("\t");
  Serial.print(RightMotor); 
  Serial.print("\t");
//  Serial.println(angle); 
//  myservo.write(angle);           
  if(LeftMotor==0){
    m.motor(2,BRAKE,LeftMotor);
    m.motor(1,BRAKE,LeftMotor);
    }
    
  else if(LeftMotor>0){
    m.motor(2,FORWARD,LeftMotor);
    m.motor(1,FORWARD,LeftMotor);
    }
    
  else{
    m.motor(2,BACKWARD,abs(LeftMotor));
    m.motor(1,BACKWARD,abs(LeftMotor));
    }

  if(RightMotor==0){
    m.motor(3,BRAKE,RightMotor);
     m.motor(4,BRAKE,RightMotor);
    }
  else if(RightMotor>0){
    m.motor(3,FORWARD,RightMotor);
    m.motor(4,FORWARD,RightMotor);
    }
  else{
    m.motor(3,BACKWARD,abs(RightMotor));
    m.motor(4,BACKWARD,abs(RightMotor));
    }
}
  
  
