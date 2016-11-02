
Servo wheels; 
Servo esc; 
double speed;
double wheelOffset;
double output;

void setup(){
  wheels.attach(D1); 
  esc.attach(D0); 
}

void controlWheel(const char *event, const char *data)
{
    var wheelOffset = output*5 * sin(90);
    //wheelOffset = double(data);// String.toDouble()???
    wheels.write(wheelOffset);
    //Try check by following
    Particle.publish(String "currWheelOffset", String String(data), double wheelOffset);
}

void controlSpeed(const char *event, const char *data)
{
    speed = double(data);
    esc.write(speed); 
    //Try check by following
    Particle.publish(String "currSpeed", String String(data), double speed);
}

// Could use for both "stop" buttom & don't hith the wall feature.
// Automatically not gonna work if detected the car already stopped.
void immediatelyStop(const char *event, const char *data)
{
    if(speed != 0){
        speed = 0;
        esc.write(0); 
    }
}

//Work for the "start" buttom. 
//Automatically not gonna work if detected the car already in speed.
void defaultStart(const char *event, const char *data)
{
    if(speed == 0){
        speed = 20;
        esc.write(speed); 
    }
    
}

void setup()
{
    Particle.subscribe("controlWheel", controlWheel);
    Particle.subscribe("controlSpeed", controlSpeed);
    Particle.subscribe("dontHit", immediatelyStop);
    Particle.subscribe("start", defaultStart)
    Serial.begin(9600);
}
