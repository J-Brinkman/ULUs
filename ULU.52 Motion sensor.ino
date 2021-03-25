/* ULU.52 Motion sensor - program  */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

#define THRES 25
#define SENSOR A3
#define RELAY 7
#define HEART 13

void setup() {
   Serial.begin(9600);
   pinMode(RELAY, OUTPUT);
   pinMode(HEART, OUTPUT);
   /* Every ULU with an Arduino introduces itself. This one is ashamed for the simplicity, so stays quiet*/ 
};

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s heartbeat for the onboard led
   Serial.println(analogRead(SENSOR));         // debug sensor value
   digitalWrite(RELAY, (analogRead(SENSOR) > THRES) ? LOW : HIGH);
}
