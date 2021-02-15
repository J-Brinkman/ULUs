/* ULU.12 Up- & down counter - program code */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

int count, lastcount, i, sumu, sumd; // integers
int up, down, lastup, lastdown; // signals
const bool binair[40][5] = {{0,0,0,0,0}, {0,0,0,0,1}, {0,0,0,1,0}, {0,0,0,1,1}, {0,0,1,0,0}, {0,0,1,0,1}, {0,0,1,1,0}, {0,0,1,1,1}, {0,1,0,0,0}, {0,1,0,0,1}, {0,1,0,1,0}, {0,1,0,1,1}, {0,1,1,0,0}, {0,1,1,0,1}, {0,1,1,1,0}, {0,1,1,1,1}, {1,0,0,0,0}, {1,0,0,0,1}, {1,0,0,1,0}, {1,0,0,1,1}, {1,0,1,0,0}, {1,0,1,0,1}, {1,0,1,1,0}, {1,0,1,1,1}, {1,1,0,0,0}, {1,1,0,0,1}, {1,1,0,1,0}, {1,1,0,1,1}, {1,1,1,0,0}, {1,1,1,0,1}, {1,1,1,1,0}, {1,1,1,1,1}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}};
const bool left[40][3] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {0,1,1}, {0,1,1}, {0,1,1}, {0,1,1}, {0,1,1}, {0,1,1}, {0,1,1}, {0,1,1}, {0,1,1}, {0,1,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,0,1}};
const bool right[40][7] = {{1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}, {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}, {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}, {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}};

#define BOUNCE 8
#define MAX 39
#define MIN 0
#define L1 1
#define L2 0
#define L3 2
#define SET0 3
#define SET1 4
#define SET2 5
#define SET3 6
#define SET4 7
#define RA 8
#define RB 9
#define RC 10
#define RD 11
#define RE 12
#define RF 13
#define RG A0
#define O4 A1
#define O3 A2
#define O2 A3
#define O1 A4
#define O0 A5
#define CD A6
#define CU A7

void setup() {
   pinMode(SET0, INPUT);
   pinMode(SET1, INPUT);
   pinMode(SET2, INPUT);
   pinMode(SET3, INPUT);
   pinMode(SET4, INPUT);
   pinMode(RA, OUTPUT);
   pinMode(RB, OUTPUT);
   pinMode(RC, OUTPUT);
   pinMode(RD, OUTPUT);
   pinMode(RE, OUTPUT);
   pinMode(RF, OUTPUT);
   pinMode(RG, OUTPUT);
   pinMode(L1, OUTPUT);
   pinMode(L2, OUTPUT);
   pinMode(L3, OUTPUT);
   pinMode(O4, OUTPUT);
   pinMode(O3, OUTPUT);
   pinMode(O2, OUTPUT);
   pinMode(O1, OUTPUT);
   pinMode(O0, OUTPUT);
   pinMode(CD, INPUT);
   pinMode(CU, INPUT);

   /* Every ULU with an Arduino introduces itself. This one uses the 7-segment display */ 
   digitalWrite(RA, HIGH);digitalWrite(RB, HIGH);digitalWrite(RD, HIGH);digitalWrite(RE, HIGH); 
   digitalWrite(RG, HIGH);delay(1000); digitalWrite(L1, HIGH); delay(1000);
   digitalWrite(RA, LOW); digitalWrite(RB, LOW);  digitalWrite(RD, LOW); digitalWrite(RE, LOW); digitalWrite(RG, LOW);
   digitalWrite(L1, LOW); delay(1000);   count = 0; lastcount = 40; lastup = LOW; lastdown = LOW;
}

void loop(){
   /* read & process input */
   sumu = 0; sumd = 0;
   for (i = 0; i < BOUNCE; i++) {sumu += analogRead(CU); sumd += analogRead(CD); } // eliminate bouncing 
   up = (sumu > (i * 512)) ? HIGH : LOW; down = (sumd > (i * 512)) ? HIGH : LOW;
   if ((down == HIGH) && (up == HIGH) && ((lastdown == LOW) || (lastup == LOW))) {
      count =  digitalRead(SET0) + (digitalRead(SET1) * 2) + (digitalRead(SET2) * 4) + (digitalRead(SET3) * 8) + (digitalRead(SET4) * 16);} 
   if ((up == HIGH) && (lastup == LOW)) count +=1;
   if ((down == HIGH) && (lastdown == LOW)) count -=1;
   lastup = up; lastdown = down;
   if (count >  MAX) count = MIN; if (count <  MIN) count = MIN;

   /* write output left[40][123] right[40][abcdefg] */
   if (count != lastcount) {
      digitalWrite(L1, left[count][0]);
      digitalWrite(L2, left[count][1]);
      digitalWrite(L3, left[count][2]);
      digitalWrite(RA, right[count][0]);
      digitalWrite(RB, right[count][1]);
      digitalWrite(RC, right[count][2]);
      digitalWrite(RD, right[count][3]);
      digitalWrite(RE, right[count][4]);
      digitalWrite(RF, right[count][5]);
      digitalWrite(RG, right[count][6]);
      digitalWrite(O4, binair[count][0]);
      digitalWrite(O3, binair[count][1]);
      digitalWrite(O2, binair[count][2]);
      digitalWrite(O1, binair[count][3]);
      digitalWrite(O0, binair[count][4]);
   }
   lastcount = count;
}
