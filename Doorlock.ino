/*
  Copyright (C) 2014 DaKnOb <daknob@tolabaki.gr>
  Code for ToLABaki <saloni@tolabaki.gr> http://tolabaki.gr/
*/

/* Configuration */
static const int StepsToRotate = 1600; /* How many steps back and FoRTH to move */
static const int SerialOpen = 0x05;    /* Serial byte to open the door at */
static const int SerialAck = 0x06;     /* Serial byte to send on packet receipt */
static const int SerialNack = 0x15;    /* Serial byte to send on packet decoding issue */
static const int SerialOk = 0x11;      /* Serial byte to send upon success */
static const int SerialNo = 0x12;      /* Serial byte to send upon failure */

/* Pins */
static const int Step = 2;   /* The pin to send a step pulse */
static const int Dir = 3;    /* The pin to send a direction logical value */
static const int Error = 5;  /* The pin to send a blink pattern on error */
static const int Switch = 6; /* The pin for the switch inside ToLABaki */

/*****************************
* No editing required below  *
* this line here.            *
*****************************/

/* Define some easy names */
#define OPEN 1
#define CLOSED 0
#define CLOCKWISE 1
#define CCLOCKWISE 0

/* Declare all variables */
int state = CLOSED;
int sBuff = 0;

/* Set appropriate pin modes */
void setup(){
  pinMode(Step, OUTPUT);
  pinMode(Dir, OUTPUT);
  pinMode(Error, OUTPUT);
  pinMode(Switch, INPUT);
  Serial.begin(9600);      /* Initialize Serial connection with a computer */
}

/* Run the actual code to use sensors for open and close */
void loop(){
    if (Serial.available() > 0){      /* Full Sensoring */
        sBuff = Serial.read();        /* Read one byte from the Serial port */
        if(sBuff == SerialOpen){
             if(openDoor()){
                Serial.write(SerialOk); 
             }else{
                Serial.write(SerialNo);
             }
        }else{
             Serial.write(SerialNack); 
        }
    }
    if(digitalRead(Switch) == HIGH){  /* Door opens using internal switch */
        openDoor();
    }
}

int openDoor(){ /* Opens door if not open */
  if(state==OPEN){
     displayError();
     return 0; /* Already open */ 
  }else{
     state = OPEN;
     rotateMotor(CLOCKWISE); /* We turn the gear clockwise, so the other gear spins counterclockwise */
     return 1;
  }
}

int closeDoor(){ /* Closes door if not closed */
  if(state==CLOSED){
     displayError();
     return 0; /* Already closed */ 
  }else{
     state = CLOSED;
     rotateMotor(CCLOCKWISE); /* We turn the gear cclockwise, so the other gear spins clockwise */
     return 1;
  }
}

void toggleDoor(){ /* Changes the door lock state */
    if(state==CLOSED){
       openDoor(); 
    }else{
       closeDoor(); 
    }
    return;
}

void forceDoorState(int st){ /* Force a door state; Use with caution */
    state = st;
    if(st == CLOSED){
       closeDoor(); 
    }else{
       openDoor(); 
    }
    return;
}

void displayError(){ /* Display an 1 second error blink on the Error LED */
   for(int i=1;i<=10;i++){
      digitalWrite(Error, HIGH);
      delay(100);
      digitalWrite(Error, LOW);
      delay(100); 
   }
}

void rotateMotor(int direction){ 	/* Spin the motor a known amount of times */
	if(direction == CLOCKWISE){
            digitalWrite(Dir, LOW);
            sendRotatePulse();
        }else{
            digitalWrite(Dir, HIGH);
            sendRotatePulse();
        }
        
}

void sendRotatePulse(){
  for(int i=1;i<=StepsToRotate;i++){
     digitalWrite(Step, HIGH);
     delay(1);
     digitalWrite(Step, LOW);
     delay(1); 
  }
  return;
}
