/* ULU.11 Clock generator - program code */
/* CC BY-NC-SA Jeroen Brinkman */ 

int i, freq, status, last_status, done, input; // integers
int signal1, signal2, signal3,signalA, signalB, last_rot, last_cont, last_pulse,  last_toggle, last_push, out, last_out; // signals
bool inp_toggle, inp_cont, inp_push,  inp_pulse, inp_manual; // booleans
unsigned long check, lastcheck, flip, lastflip; // milliseconds
int ms [22] = {10000, 7500, 5000, 4500, 4000, 3500, 3000, 2500, 2000, 1500, 1000, 500, 450, 400, 350, 300, 250, 200, 150, 100, 50, 25};
int state [16][4] = {{0, 1, 2, 0}, {1, 1, 2, 1}, {2, 2, 0, 2}, {2, 2, 0, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}};

#define BOUNCE 8
#define MAX 21
#define EEN 1
#define TOGGLE 2
#define CONT 3
#define PULSE 4
#define MANUAL 5
#define ROTA 6 // Use ROTA 7 if right turn does decrease frequency
#define ROTB 7 // Use ROTB 6 if right turn does decrease frequency
#define ROTP 8
#define INDICATE 9
#define CLOCKON 10
#define CLOCK 11
#define HEART  13
#define CHECK  180

void setup() {
   pinMode(EEN, OUTPUT);
   pinMode(CONT, INPUT);
   pinMode(TOGGLE, INPUT);
   pinMode(PULSE, INPUT);
   pinMode(MANUAL, INPUT_PULLUP);
   pinMode(ROTA, INPUT_PULLUP);
   pinMode(ROTB, INPUT_PULLUP);
   pinMode(ROTP, INPUT_PULLUP);
   pinMode(INDICATE, OUTPUT);
   pinMode(CLOCKON, OUTPUT);
   pinMode(CLOCK, OUTPUT);
   pinMode(HEART,  OUTPUT); // blinking led showing the programma's hartbeat
   freq = 10; status = 0;
   last_toggle = LOW;  last_pulse = LOW; last_rot = LOW; last_push = LOW, out = LOW, last_out=LOW;
   lastcheck = millis(); flip = millis();
   /* Every ULU with an Arduino introduces itself. This one uses Blinking LEDs */ 
   digitalWrite(CLOCKON, HIGH); delay(100); digitalWrite(CLOCKOB, LOW); delay(400);
   digitalWrite(INDICATE, HIGH); delay(100); digitalWrite(INDICATE, LOW); delay(400);
}

void loop() {
   digitalWrite(EEN, (millis() / 1000) % 2); //1s heartbeat for the onboard led

   /* read manual push */
   inp_manual  = (digitalRead(MANUAL) == LOW); // Manual pulse

   /* read debounce */
   signal1 = 0; signal2 = 0; signal3 = 0; signalA = 0; signalB = 0;
   for (int i = 0; i < BOUNCE; i++) { // eliminate bouncing 
      signal1 += digitalRead(CONT);
      signal2 += !digitalRead(ROTP); 
      signal3 += digitalRead(TOGGLE); 
      signalA += !digitalRead(ROTA); 
      signalB += !digitalRead(ROTB); 
      delay(1);
    } 

   /* on*/
   signal1 = (signal1 > i / 2) ? HIGH : LOW;
   inp_cont = (signal1 == HIGH);
   inp_toggle = ((last_cont == HIGH) && (signal1 == LOW));
   last_cont = signal1;

   /* toggle */
   signal2 = (signal2 > i / 2) ? HIGH : LOW;
   inp_toggle = inp_toggle || ((last_push == LOW) && (signal2 == HIGH));
   last_push = signal2;

   signal3 = (signal3 > i / 2) ? HIGH : LOW;
   inp_toggle =  inp_toggle || ((last_toggle == LOW) && (signal3 == HIGH));
   last_toggle = signal3;

   /* single pulse */
   signal1 = digitalRead(PULSE); // Single pulse when 1
   inp_pulse = ((last_pulse == LOW) && (signal1 == HIGH) );
   last_pulse = signal1;

   /* read rotary function */
   signalA = (signalA > i / 2) ? HIGH : LOW;
   signalB = (signalB > i / 2) ? HIGH : LOW;
   if ((last_rot == LOW) && (signalA == HIGH)) {
      if (signalB == LOW) {
         freq = freq - 1; if (freq < 0) freq = 0;
         digitalWrite(INDICATE, HIGH); delay(30);
         digitalWrite(INDICATE, LOW); 
      } else {
         freq = freq + 1; if (freq > MAX) freq = MAX;
         digitalWrite(INDICATE, HIGH); delay(30);
         digitalWrite(INDICATE, LOW); delay(30);
         digitalWrite(INDICATE, HIGH); delay(30);
         digitalWrite(INDICATE, LOW); 
      }
   }
   last_rot = signalA;
   digitalWrite(INDICATE, ((freq == MAX) || (freq == 0) || (freq == 10) ) ? HIGH : LOW);

   /* determine new clock status */
   // 0 - clock off
   // 1 - single pulse
   // 2 - clock on (pulse train)
   // 3 - out high (when manual button is pressed)
   input = 0;
   input += inp_manual ? 8 : 0;
   input += inp_cont ? 4 : 0;
   input += inp_toggle ? 2 : 0;
   input += inp_pulse ? 1 : 0;
   last_status = status;
   status = state[input][status];
   if (last_status != status) flip = millis();

   /* enable/disable clock according to status */
   check = millis() - lastcheck; // time needed for one check
   lastcheck = millis() ;
   switch (status) {
   case 0: // clock off
      out = LOW; done = 0;
      break;
   case 1: // single pulse
      if ((millis() < flip) && (millis() + check > flip)) delay(flip - millis());
      if (millis() >= flip) { // flip the output
         out = (out == HIGH) ? LOW : HIGH;
         flip = flip + (ms[freq]);
      }
      if ((done == 0) && (out == HIGH)) done = 1;
      if ((done == 1) && (out == LOW)) done = 2;
      if ((done == 2) && (out == HIGH)) {
        done = 3; out = LOW;
      }
      if ((done == 3) && !inp_pulse) status = 0;

      break;
   case 2: // clock on
      if ((millis() < flip) && (millis() + check > flip)) delay(flip - millis());
      if (millis() >= flip) { // flip the output
         out = (out == HIGH) ? LOW : HIGH;
         flip = flip + (ms[freq]);
      }
      break;
   case 3: // out high
      out = HIGH;
      break;
   }
   digitalWrite(CLOCKON, (status == 2));
   digitalWrite(HEART, out);
   digitalWrite(CLOCK, out);
}
