#include <Servo.h>
//Motor driver pins
int in1 = 24; //FWD RIGHT
int in2 = 26; //BWD RIGHT
int in3 = 28; //BWD LEFT
int in4 = 30; //FWD LEFT
int enA = 4; //LEFT
int enB = 5; //RIGHT

//Sensor pins
int dist_front = A0;
int dist_arm = A1;
int line_left = A8;
int line_mid = A9;
int line_right = A10;

Servo joint_0;
Servo joint_1;
Servo joint_2;
Servo joint_3;


//Global variables
int state = 0;
int j1 = 0;
int j2 = 0;
int j3 = 0;
int j0 = 0;
int ln_left;
int ln_mid;
int ln_right;

void GoForward(int vel = 255){
  digitalWrite(in1,HIGH); //FWD RIGHT
  digitalWrite(in2,LOW); //BWD RIGHT
  digitalWrite(in3,LOW); //BWD LEFT
  digitalWrite(in4,HIGH); //FWD LEFT
  analogWrite(enA,vel);  //LEFT
  analogWrite(enB,vel);//RIGHT
  delay(10);
}

void RotateRight(int vel = 255){
  digitalWrite(in1, LOW); //FWD RIGHT
  digitalWrite(in2,HIGH); //BWD RIGHT
  digitalWrite(in3,LOW); //BWD LEFT
  digitalWrite(in4,HIGH); //FWD LEFT
  analogWrite(enA,vel);  //LEFT
  analogWrite(enB,vel);//RIGHT
  delay(10);
}

void RotateLeft(int vel = 255){
  digitalWrite(in1,HIGH); //FWD RIGHT
  digitalWrite(in2,LOW); //BWD RIGHT
  digitalWrite(in3,HIGH); //BWD LEFT
  digitalWrite(in4,LOW); //FWD LEFT
  analogWrite(enA,vel);  //LEFT
  analogWrite(enB,vel);//RIGHT
  delay(10);
}


void GoBackward(int vel = 255){
  digitalWrite(in1,LOW); //FWD RIGHT
  digitalWrite(in2,HIGH); //BWD RIGHT
  digitalWrite(in3,HIGH); //BWD LEFT
  digitalWrite(in4,LOW); //FWD LEFT
  analogWrite(enA,vel);  //LEFT
  analogWrite(enB,vel);//RIGHT
  delay(10);
}

void Stop(){
  digitalWrite(in1,LOW); //FWD RIGHT
  digitalWrite(in2,LOW); //BWD RIGHT
  digitalWrite(in3,LOW); //BWD LEFT
  digitalWrite(in4,LOW); //FWD LEFT
  analogWrite(enA,0);  //LEFT
  analogWrite(enB,0);//RIGHT
  delay(10);
}
void FindObject(){
    analogRead(dist_front);
    delay(1);
    int dist_val = analogRead(dist_front);
    if(dist_val<150){
     RotateRight(255);  
    }
    else if(dist_val>=150  && dist_val<400){
      GoForward(255);
    }
    else if(dist_val>450){
      GoBackward(255);
    }
    else{
      Stop();
      state = 1;  
    }
}

void ArmHome(){
  j0=110;
  j1=150;
  j2=40;
  j3=0;
  joint_0.write(j0);
  joint_1.write(j1);
  joint_2.write(j2);
  joint_3.write(j3);
  delay(10);
  }

void ArmLower(){

  joint_0.write(110);
  joint_1.write(140);
  joint_2.write(48);
  joint_3.write(25);
  delay(300);

  joint_0.write(110);
  joint_1.write(130);
  joint_2.write(56);
  joint_3.write(50);
  delay(300);
  joint_0.write(110);
  joint_1.write(120);
  joint_2.write(64);
  joint_3.write(50);
  delay(300);
  joint_0.write(110);
  joint_1.write(110);
  joint_2.write(72);
  joint_3.write(50);
  delay(300);
  joint_0.write(110);
  joint_1.write(100);
  joint_2.write(80);
  joint_3.write(50);
  j0 = 110;
  j1 = 100; 
  j2 = 80; 
  j3 = 50; 
  delay(1000);
  }

void GrabObject(){
  int dist_val = analogRead(dist_arm);
  delay(10);
  if(dist_val<600){
    j2--;
    joint_2.write(j2);
    delay(50);
    if(j2<=0){
      ArmHome();
      state=0;
      }
    }
  else{
    j2 = j2-15;
    j3=0;
    joint_2.write(j2);
    delay(50);
    joint_3.write(j3);
    delay(1000);
    state = 4;
    }
  }


void LocateObject(){
  int dist_val;
  int i = 80;
  int max_val = 0;
  int max_index = 0;
  for(i=80;i<=140;i++){
    joint_0.write(i);
    delay(50);
    dist_val = analogRead(dist_arm);
    delay(5);
    if(dist_val>max_val){
      max_val = dist_val;
      max_index = i; 
      }
    }
    j0=max_index;
    joint_0.write(j0);
    delay(1000);
    state=3;
  }

void UpdateLine(){
  ln_left = analogRead(line_left);
  delay(1);
  if(ln_left<250){
      ln_left = 1;
  }
  else{
    ln_left = 0;
  }
  ln_right = analogRead(line_right);
  delay(1);
  if(ln_right<250){
      ln_right = 1;
  }
  else{
    ln_right = 0;
  }
  ln_mid = analogRead(line_mid);
  delay(1);
  if(ln_mid<250){
      ln_mid = 1;
  }
  else{
    ln_mid = 0;
  }  
}

void r_Forward(int vel = 255){
  digitalWrite(in1,HIGH); //FWD RIGHT
  digitalWrite(in2,LOW); //BWD RIGHT
  analogWrite(enB,vel);//RIGHT
  delay(10);
}

void l_Forward(int vel = 255){
  digitalWrite(in3,LOW); //BWD LEFT
  digitalWrite(in4,HIGH); //FWD LEFT
  analogWrite(enA,vel);  //LEFT
  delay(10);
}

void r_Stop(){
  digitalWrite(in1,LOW); //FWD RIGHT
  digitalWrite(in2,LOW); //BWD RIGHT
  analogWrite(enB,0);//RIGHT
  delay(10);
}

void l_Stop(){
  digitalWrite(in3,LOW); //BWD LEFT
  digitalWrite(in4,LOW); //FWD LEFT
  analogWrite(enA,0);  //LEFT
  delay(10);
}
void FollowLine(){
  UpdateLine();
  if(ln_left == 0 && ln_mid ==0 && ln_right==0){
    Stop();
  }
  else if(ln_left == 0 && ln_mid == 1 && ln_right==0){
    Stop();
    state=7;
  }
  else{
    if(ln_left==1){
      l_Forward();
    }
    else{
      l_Stop();
    }
    if(ln_right==1){
      r_Forward();
    }
    else{
      r_Stop();
    }
  }
}

void FindLine(){
  UpdateLine();
  if(ln_left == 0 && ln_mid == 0 && ln_right ==0){
    GoForward();
    delay(100);
    Stop();
    delay(500);
    UpdateLine();
    while(ln_mid==1){
    RotateRight();
    UpdateLine();
    }
    Stop();
    state = 6;
  }
  else{
    GoForward();
  }
}

void Go2Center(){
    GoBackward();
    delay(500);
    RotateRight();
    delay(500);
    ArmHome();
    GoForward();
    delay(3000);
    state = 0;
}

void setup() {
  //attach arm servos
  joint_0.attach(8);
  joint_1.attach(9);
  joint_2.attach(10);
  joint_3.attach(11);


  //Set I/O pins
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);

  
  //stop motors
  digitalWrite(in1,LOW); //FWD RIGHT
  digitalWrite(in2,LOW); //BWD RIGHT
  digitalWrite(in3,LOW); //BWD LEFT
  digitalWrite(in4,LOW); //FWD RIGHT
  analogWrite(enA,0);  //LEFT
  analogWrite(enB,0);//RIGHT
  
  //Home all joints
  j0=110;
  j1=150;
  j2=40;
  j3=10;
  joint_0.write(j0);
  joint_1.write(j1);
  joint_2.write(j2);
  joint_3.write(j3);

  delay(5000);
  //Serial.begin(9600);
  //ArmLower();
}


void loop() {
  // put your main code here, to run repeatedly:
  
  switch(state){
    case 0: FindObject();
            break;
    case 1: ArmLower();
            state = 2;
            break;
    case 2: LocateObject();
            break;
    case 3: GrabObject();
            break;
    case 4: ArmHome();
            state=5;
            break;
    case 5: FindLine();
            break;
    case 6: FollowLine();
            break;
    case 7: ArmLower();
            Go2Center();
            break;
    default:Stop();
            break;
  }
 
/*
    analogRead(dist_front);
    delay(1);
int dist_val = analogRead(dist_front);
Serial.print(dist_val);
Serial.print("\n");
*/

}
