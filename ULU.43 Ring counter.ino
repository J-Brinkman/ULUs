/* ULU.43 Ring counter - program code  */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

int i, signa, value; // integers
int maximum; // 3-bit words
bool reset, count, lastcount; // booleans

#define BOUNCE 8
#define RELAYR 2
#define RELAYL 3
#define RELAY1 4
#define RELAY2 5
#define COUNT 6
#define RESET 7
#define MAX2 8
#define MAX1 9
#define MAX0 10
#define HEART 13

void setup() {
   pinMode(RELAYR, OUTPUT);
   pinMode(RELAYL, OUTPUT);
   pinMode(RELAY1, OUTPUT);
   pinMode(RELAY2, OUTPUT);
   pinMode(COUNT, INPUT);
   pinMode(RESET, INPUT);
   pinMode(MAX2, INPUT);
   pinMode(MAX1, INPUT);
   pinMode(MAX0, INPUT);
   pinMode(HEART, OUTPUT);
   /* Every ULU with an Arduino introduces itself. This one is speachless, so stays quiet*/ 

   lastcount = false; value = 1;
};

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s heartbeat for the onboard led

   /* Read all input  */ 
   maximum = digitalRead(MAX0) + (digitalRead(MAX1) * 2) + (digitalRead(MAX2) * 4); 
   if (maximum == 0) maximum = 9;
   reset = (digitalRead(RESET) == HIGH);
   signa = 0; 
   for (int i = 0; i < BOUNCE; i++) {signa += digitalRead(COUNT); delay(2);} // eliminate bouncing 
   count = (signa > (i / 2));

   /* Process input  */ 
   if (!count && lastcount) value += 1;
   if (value == 9) value = 1;
   if (value > maximum) value = 1;
   if (reset) value = 1;

   /* Write output  */ 
   switch (value) {
      case 1: // 000
         digitalWrite(RELAYL, LOW);
         digitalWrite(RELAYR, LOW);
         digitalWrite(RELAY1, LOW);
         digitalWrite(RELAY2, LOW);
         break;
      case 2: // 001
         digitalWrite(RELAYL, HIGH);
         digitalWrite(RELAYR, HIGH);
         digitalWrite(RELAY1, LOW);
         digitalWrite(RELAY2, LOW);
         break;
      case 3: // 010
         digitalWrite(RELAYL, LOW);
         digitalWrite(RELAYR, LOW);
         digitalWrite(RELAY1, HIGH);
         digitalWrite(RELAY2, LOW);
         break;
      case 4: // 011
         digitalWrite(RELAYL, HIGH);
         digitalWrite(RELAYR, HIGH);
         digitalWrite(RELAY1, HIGH);
         digitalWrite(RELAY2, LOW);
         break;
      case 5: // 100
         digitalWrite(RELAYL, LOW);
         digitalWrite(RELAYR, LOW);
         digitalWrite(RELAY1, LOW);
         digitalWrite(RELAY2, HIGH);
         break;
      case 6: // 101
         digitalWrite(RELAYL, HIGH);
         digitalWrite(RELAYR, HIGH);
         digitalWrite(RELAY1, LOW);
         digitalWrite(RELAY2, HIGH);
         break;
      case 7: // 110
         digitalWrite(RELAYL, LOW);
         digitalWrite(RELAYR, LOW);
         digitalWrite(RELAY1, HIGH);
         digitalWrite(RELAY2, HIGH);
         break;
      case 8: // 111
         digitalWrite(RELAYL, HIGH);
         digitalWrite(RELAYR, HIGH);
         digitalWrite(RELAY1, HIGH);
         digitalWrite(RELAY2, HIGH);
         break;
   }
   lastcount = count;
}
