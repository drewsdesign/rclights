/***********************
* RC Controlled Lights *
***********************/


/* DEFINE INPUTS AND OUTPUT PINS */
const int ledPins[] = { 
  11, 10, 9, 6                              /* Array of led pins */
};                
const int pinCount = 4;                     /* Total number of leds */
const int buttonPin = 2;                    /* Pin connected to tact switch */
const int txSwitch = 5;                     /* Pin used for the receiver signal */
                
/* DEFINE VARIABLES */                       
volatile int RXSG;                          /* Length of RX PWM signal */
int RXOK, PWMSG;                
int ledSequence = 0;                        /* Counter for the number of button presses */
int buttonState = 0;                        /* Current state of the button */
int lastButtonState = 0;                    /* Previous state of the button */
long time = 0;                              /* The last time the output pin was toggled */
long debounce = 200;                        /* The debounce time, increase if the output flickers */
long randm = 0;                             /* Random value for disco function */


void setup()  {  
  delay(1000); 
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);      /* Setup IO pins */  
  }
  pinMode(txSwitch, INPUT);
  pinMode(buttonPin, INPUT);  
  randomSeed(analogRead(0));                /* Initial random value from pin 0 */  
  Serial.begin(9600);                       /* Initialize serial communications for testing */
} 


void loop()  { 
  checkBtnPress();
  if (ledSequence == 0) { 
    RXSG = pulseIn(txSwitch, HIGH, 20000);  /* Receiver PWM changes mode */
    if (RXSG== 0) {
      RXSG = RXOK;
    } 
    else {
      RXOK = RXSG;
    }
    PWMSG = map(RXSG, 1000, 2000, 0, 511);  /* Substitute the high values to a value between 0 and 511 */
    constrain (PWMSG, 0, 511);              /* Make sure that the value stays within the disired boundries */

    Serial.print("PWMSG: ");                /* Print RX values for testing */
    Serial.print(PWMSG);
    Serial.print(" / RXSG: ");  
    Serial.print(RXSG);
    delay(5); 
    Serial.println();

    if (PWMSG >= 400) {
      ledsOff();                            /* TX switch at position 1 */
    }
    else if (PWMSG < 400 && PWMSG > 100) {
      ledsOn();                             /* TX switch at position 2 */
    }
    else if (PWMSG <= 100) {
      airStrobe(3000);                      /* TX switch at position 3 */
    }
  }
  else {
    switch (ledSequence) {                  /* Cycle sequence from button press */
    case 1:
      ledsOn();
      break;
    case 2:
      chase(3);
      break;
    case 3:
      fadeInOut(10);
      break;
    case 4:
      disco(20, 120);
      break;
    case 5:
      airStrobe(4000);
      break;
    default:
      ledSequence = 0;                      /* Reset count and listen for TX switch input */
      ledsOff();
    }        
  }

}

void checkBtnPress() {  
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && lastButtonState == LOW && millis() - time > debounce) {
    ledSequence++;
    Serial.println(ledSequence);
    time = millis();
  }
  lastButtonState = buttonState;            /* Save state */
}

void ledsOff() {
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {   
    digitalWrite(ledPins[thisPin], LOW);    /* Turn off all leds */
    delay(1000);                            /* Debounce to prevent accidental button press */
  }
}

void ledsOn() {
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    digitalWrite(ledPins[thisPin], HIGH);   /* Turn on all leds */
  }
}

void fadeInOut(int t) {
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    int fadeInverse = 255 - fadeValue;
    analogWrite(ledPins[0], fadeValue);
    analogWrite(ledPins[1], fadeValue);
    analogWrite(ledPins[2], fadeInverse);
    analogWrite(ledPins[3], fadeInverse);
    delay(t);
  }
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    int fadeInverse = 255 - fadeValue;
    analogWrite(ledPins[0], fadeInverse);
    analogWrite(ledPins[1], fadeInverse);
    analogWrite(ledPins[2], fadeValue);
    analogWrite(ledPins[3], fadeValue);
    delay(t);
  }
}


void airStrobe(int t) {
  static unsigned long previousUpdateTime;
  if(millis()-previousUpdateTime > t){
    previousUpdateTime = millis();
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    delay(50);   
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], LOW);
    delay(100);   
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    delay(50);
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], LOW);
    delay(10);
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], HIGH);
    delay(50);   
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[3], LOW);
    delay(100);   
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], HIGH);
    delay(50);
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[3], LOW);
  }
}

void disco(int on, int off) {
  static unsigned long previousUpdateTime;
  randm = random(on, off);
  if(millis()-previousUpdateTime > randm){
    previousUpdateTime = millis();
    for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
      digitalWrite(ledPins[thisPin], LOW);      
    }  
    int randLed = random(4);
    digitalWrite(ledPins[randLed], HIGH);
  }
}

void chase(int t) {
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    analogWrite(ledPins[3], fadeValue);
    delay(t);                            
  }  
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[3], LOW);
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    analogWrite(ledPins[0], fadeValue);
    delay(t);                            
  }  
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[2], HIGH);
  digitalWrite(ledPins[3], LOW);
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    analogWrite(ledPins[1], fadeValue);
    delay(t);                            
  }  
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[3], HIGH);  
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    analogWrite(ledPins[2], fadeValue);
    delay(t);                            
  }
}
