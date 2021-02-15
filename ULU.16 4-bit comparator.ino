/* ULU.16 4-bit comparator - program code */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

int left, right, check; // integers
int output, less, equal, greater; // signals
bool overflow; // booleans

#define LE0  3
#define LE1  4
#define LE2  5
#define LE3  6
#define OVER  7
#define RI0  8
#define RI1  9
#define RI2  11
#define RI3  10
#define TWOCO  12
#define HEART  13
#define CHK0  A0
#define CHK1  A1
#define OUT  A2
#define  ISG A3
#define  ISE A4
#define  ISL A5

void setup() {
   pinMode(LE0, INPUT);
   pinMode(LE1, INPUT);
   pinMode(LE2, INPUT);
   pinMode(LE3, INPUT);
   pinMode(RI0, INPUT);
   pinMode(RI1, INPUT);
   pinMode(RI2, INPUT);
   pinMode(RI3, INPUT);
   pinMode(OVER, INPUT);
   pinMode(CHK0, INPUT);
   pinMode(CHK1, INPUT);
   pinMode(TWOCO, INPUT_PULLUP);
   pinMode(HEART,  OUTPUT); // blinking led showing the programma's hartbeat
   pinMode(OUT, OUTPUT);
   pinMode(ISL, OUTPUT);
   pinMode(ISE, OUTPUT);
   pinMode(ISG, OUTPUT);

   /* Every ULU with an Arduino introduces itself. This one uses Blinking LEDs */ 
   digitalWrite(ISE, HIGH); digitalWrite(ISG, HIGH); delay(100);
   digitalWrite(ISE, LOW); digitalWrite(ISG, LOW); delay(100);
   digitalWrite(ISG, HIGH); delay(100); digitalWrite(ISG, LOW); delay(100);
   digitalWrite(ISG, HIGH); delay(100); digitalWrite(ISG, LOW); delay(100);
   digitalWrite(ISG, HIGH); delay(100); digitalWrite(ISG, LOW); delay(100);
   digitalWrite(ISG, HIGH); delay(100); digitalWrite(ISG, LOW); delay(100);
   digitalWrite(ISG, HIGH); delay(100); digitalWrite(ISG, LOW); delay(100);
}

void loop(){
    digitalWrite(HEART, (millis() / 1000) % 2); //1s heartbeat for the onboard led

   /* Read all input */ 
   check = (digitalRead(CHK1) * 2) + digitalRead(CHK0);
   overflow = (digitalRead(OVER) == HIGH);
   left =  digitalRead(LE0) + (digitalRead(LE1) * 2) + (digitalRead(LE2) * 4) + (digitalRead(LE3) * 8);  
   right =  digitalRead(RI0) + (digitalRead(RI1) * 2) + (digitalRead(RI2) * 4) + (digitalRead(RI3) * 8);

   if (digitalRead(TWOCO) == LOW){
      if (left > 7) left -= 16;
      if (right > 7) right -= 16; 
   }

   /* Perform checking */ 
   less = LOW; equal = LOW; greater = LOW; output=LOW;
   if (left < right) less = HIGH;
   if (left == right) equal = HIGH;
   if (left > right) greater = HIGH;
   switch (check) {
      case 0: // left < right
         output = less;
         break;
      case 1: // left = right
         output = equal;
         break;
      case 2: // left > right
         output = (((greater == HIGH) || overflow) ? HIGH : LOW);
         break;
      case 3: // overflow
           output = overflow ? HIGH : LOW;
      break;
      }

   /* Write output */ 
   digitalWrite(OUT, output);
   if (overflow == LOW) {
      digitalWrite(ISL, less);
      digitalWrite(ISE, equal);
      digitalWrite(ISG, greater);
   } else {
      digitalWrite(ISL, !less);
      digitalWrite(ISE, !equal);
      digitalWrite(ISG, !greater);
   }
}
