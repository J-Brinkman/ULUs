/* ULU.31 Optical tape reader - program code  */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

int i, direct, trigger, trigger_1, trigger_2, trigger_3, last, actual, sumt; // integers
int thres[5] = {975,975,975,975,500}; // threshold for the four bits 0,1,2,3 and trigger (4) difference between black (1) and white (0)


#define THRESP 700 // threshold when the trigger has detected paper
#define BOUNCE 3
#define THRES 2
#define BIT0 A4
#define BIT1 A3
#define BIT2 A2
#define BIT3 A1
#define TRIGGER A0
#define PAPER 9
#define OUT3 7
#define OUT2 6
#define OUT1 5
#define OUT0 4
#define CLOCK 8
#define HEART 13

void setup() {
   pinMode(PAPER, OUTPUT);
   pinMode(CLOCK, OUTPUT);
   pinMode(OUT0, OUTPUT);
   pinMode(OUT1, OUTPUT);
   pinMode(OUT2, OUTPUT);
   pinMode(OUT3, OUTPUT);
   pinMode(HEART, OUTPUT);

   /* Every ULU with an Arduino introduces itself. This uses a flashing LED*/ 
   digitalWrite(PAPER, HIGH); delay(100); digitalWrite(PAPER, LOW); delay(100);
   digitalWrite(PAPER, HIGH); delay(100); digitalWrite(PAPER, LOW); delay(100);
   digitalWrite(PAPER, HIGH); delay(100); digitalWrite(PAPER, LOW); delay(400);
   digitalWrite(PAPER, HIGH); delay(100); digitalWrite(PAPER, LOW); 

   trigger_3 = 0; trigger_2 = 0; trigger_1 = 0; trigger = 0;
   direct = 0;
};

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s heartbeat for the onboard led

    /* direct meaning: */
   // 0. No paper
   // 1. downwards
   // 2. bottom (black)
   // 3. upwards
   // 4. top (white)
   // Black is less light is lower number

   /* Read & process trigger  */ 
   sumt = 0;
   for (i = 0; i < BOUNCE; i++) {sumt += analogRead(TRIGGER); } // eliminate variances 
   trigger = sumt / BOUNCE;
   if (trigger <= THRESP) { // paper inserted
      if (direct == 0) direct = 1;
      digitalWrite(PAPER, HIGH);
   } else {
      direct = 0;
      digitalWrite(PAPER, LOW);
      digitalWrite(OUT0, LOW);
      digitalWrite(OUT1, LOW);
      digitalWrite(OUT2, LOW);
      digitalWrite(OUT3, LOW);
      digitalWrite(CLOCK, LOW);
   }
   actual = trigger + trigger_1 + trigger_2;
   last = trigger_1 + trigger_2 + trigger_3;

   if ((last > (actual + THRES)) && (direct == 3)) direct = 4;
   if (((last + THRES) < actual) && (direct == 1)) direct = 2;
   trigger_3 = trigger_2; trigger_2 = trigger_1; trigger_1 = trigger;

   /* Write output  */ 
   if (direct == 2) {
      direct = 3;
      digitalWrite(OUT0, (analogRead(BIT0) < thres[0] ? HIGH : LOW));
      digitalWrite(OUT1, (analogRead(BIT1) < thres[1] ? HIGH : LOW));
      digitalWrite(OUT2, (analogRead(BIT2) < thres[2] ? HIGH : LOW));
      digitalWrite(OUT3, (analogRead(BIT3) < thres[3] ? HIGH : LOW));
      digitalWrite(CLOCK, HIGH);
   }
   if (direct == 4) {
      direct = 1;
      digitalWrite(OUT0, LOW);
      digitalWrite(OUT1, LOW);
      digitalWrite(OUT2, LOW);
      digitalWrite(OUT3, LOW);
      digitalWrite(CLOCK, LOW);
   }
}
