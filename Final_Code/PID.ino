#include "pid/pid.h"

Servo wheels; // servo for turning the wheels
Servo esc; // not actually a servo, but controlled like one!
double pulse_width_front, pulse_width_back;
int lidar_front = D3;
int lidar_back = D4;
int startupDelay = 1000;
int control;
double Setpoint, Input, Output, Gap, input_raw, Offset, c_output, d_diff;
double aggKp=1, aggKi=0.0005, aggKd=0.15; 
double consKp=1, consKi=0.00005, consKd=3=0.5;
double distance;
double flag = 1, c_flag = 1;
int angle, c_angle;

PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, PID::DIRECT);
 
void setup(){

  Particle.function("start",start);
  Particle.function("stop", stop);
 
  pulse_width_front = pulseIn(D3, HIGH);
  pulse_width_back = pulseIn(D4, HIGH);
  delay(50);
  Input = abs(pulse_width_front-pulse_width_back);
  Offset = (pulse_width_front + pulse_width_back)/20;
  //Particle.publish("Connected");
 
  Setpoint = 4;
  Gap = abs(Setpoint-Input);
  myPID.SetMode(PID::AUTOMATIC);
  wheels.attach(D0);
  esc.attach(D1); 
  wheels.write(95);
  pinMode(lidar_front, INPUT); 
  pinMode(lidar_back, INPUT);
  calibrateESC();

}
 void calibrateESC(){
    esc.write(180); // full backwards
    delay(startupDelay);
    esc.write(0);
    delay(startupDelay);
    esc.write(90);
    delay(startupDelay);
    esc.write(90);
 }
 
  void oscillate(){
    distance = analogRead(A0);
   // Particle.publish("Stop",String(distance));
    if (distance >= 300 && control == 1){
        esc.write(50);
        //delay(10);
        pulse_width_front = pulseIn(D3, HIGH);
        //Particle.publish("lidar1",String(pulse_width_front));
        //delay(10);
        pulse_width_back = pulseIn(D4, HIGH);
        //Particle.publish("lidar2",String(pulse_width_back));
       // Particle.publish("Output",String(pulse_width_front) + "+" + String(pulse_width_back) + "+" +String(Output)+ "+" +String(distance));
        //delay(1000);
        if ( pulse_width_front !=0 && pulse_width_back != 0 && pulse_width_front <= 5000 && pulse_width_front <= 5000){
            pulse_width_front = pulse_width_front/10; // 10usec = 1 cm of distance for LIDAR-Lite
            //Particle.publish("lidar1",String(pulse_width_front));
            pulse_width_back = pulse_width_back/10;
            //Particle.publish("lidar2",String(pulse_width_front) + "+" + String(pulse_width_back));
  
            input_raw = pulse_width_front - pulse_width_back;
            Input =0 - abs(input_raw);

            Gap = abs( Input - Setpoint ); 
            if (Gap > 60){
                Output=0;
            }
 
            else if(Gap>25) 
            {  //we're close to setpoint, use conservative tuning parameters 
                myPID.SetTunings(consKp, consKi, consKd); 
            } 
            else 
            { 
      //we're far from setpoint, use aggressive tuning parameters 
            myPID.SetTunings(aggKp, aggKi, aggKd); 
            } 
    
            myPID.Compute(); 
            Particle.publish("Output",String(pulse_width_front) + "+" + String(pulse_width_back) + "+" +String(Output)+ "+" +String(distance));
   
            if (input_raw >= 0){
                if (pulse_width_front > Offset + 5)
                flag = -1.5;
                else flag = 0.25;
            }
            else if (pulse_width_front < Offset - 5){
                
                flag = 2;
            }
            else flag = -0.25;
            angle = flag*Output/255*10 + 94;
            wheels.write(angle);
            
            //Particle.publish("Angle",String(angle));
            }
        }
    
    else {
     esc.write(90);
    }
   // Particle.publish("control",String(control));
    //delay(1000);
   
  }                                                                                                                                                                                                              

void loop(){
    oscillate();
 
   
}
int stop(String str) {
    //Particle.publish("stop","stop");
    //int b = Particle.publish("")
    control = 0;
    return 1;
}

int start(String str) {

   control =1 ;


   // Particle.publish("start",String(control));

   return 0;
}
