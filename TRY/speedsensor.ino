// This #include statement was automatically added by the Particle IDE.
#include "SparkTime/SparkTime.h"

//#include "Time/Time.h"
int speedsensor = D2;
int control = D6;
int val =0;
time_t tst =0;
time_t ted =0;
bool flag =0;
time_t period =0;
double cons=78.5;
double s =0;

void setup() {
  pinMode(speedsensor, INPUT);
  pinMode(control, OUTPUT);
  pinMode(D7,OUTPUT);
  Particle.function("start",start);
  Particle.function("stop", stop);
  val = digitalRead(speedsensor);
  //t = millis();
 //Particle.publish("speed start", String(val));
  
}

void loop() {
    int in = digitalRead(speedsensor);   // read the input pin
   // Particle.publish("current", String(in));
    if (val != in) {
        if(flag == 0) {
            val = in;
            tst = millis();
            //Particle.publish("start", String(tst));
          flag = 1;
        }
        else {
            val = in;
            ted = millis();
           // Particle.publish("end", String(ted));
            speed(tst, ted);
            flag=0;
        }
        
        
      
    }
//delay(1000);
    

}

void speed(time_t tst, time_t ted) {
    period = ted - tst;
    s = cons/period;
    Particle.publish("speed", String(s));
    delay(1000);
}

   int stop(String str) {
    digitalWrite(control, LOW);
    digitalWrite(D7, LOW);
    Particle.publish("stop","stop");
    //int b = Particle.publish("")
    
    return 1;
}
   int start(String str) {
    digitalWrite(D7, HIGH);
    digitalWrite(control, HIGH);
   

    
    Particle.publish("start","start");
     //int a = digitalRead(control);
     //Particle.publish("test", String(a));
    
    return 0;
}






