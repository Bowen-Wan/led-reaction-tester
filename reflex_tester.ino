int LED1 = 3;
int LED2 = 6;
int LED3 = 9;
int LED4 = 12;

int button1 = 2;
int button2 = 5;
int button3 = 8;
int button4 = 11;

int buzzer = 13;

int leds[4] = {LED1, LED2, LED3, LED4};
int buttons[4] = {button1, button2, button3, button4};

int currentLED = -1;
unsigned long startTime;
unsigned long bestReactionTime = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println("--- GAME START ---");
  delay(1000); 
  lightRandomLED();
}

void lightRandomLED() {
  if (currentLED != -1) {
    digitalWrite(leds[currentLED], LOW);
  }


  int nextLED;
  do {
    nextLED = random(0, 4);
  } while (nextLED == currentLED);

  currentLED = nextLED;


  digitalWrite(leds[currentLED], HIGH);
  startTime = millis();
}

void loop() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttons[i]) == HIGH) {
      

      if (i == currentLED) {
        unsigned long reactionTime = millis() - startTime;

        //When correct button is pressed
        tone(buzzer, 1000, 100);

        if (bestReactionTime == 0 || reactionTime < bestReactionTime) {
          bestReactionTime = reactionTime;
          Serial.print("NEW BEST RECORD! ");
        } else {
          Serial.print("Correct! ");
        }

        Serial.print("Reaction: ");
        Serial.print(reactionTime);
        Serial.print(" ms | Best: ");
        Serial.print(bestReactionTime);
        Serial.println(" ms");

        // Wait for button release
        //500 ms cooldown

        while (digitalRead(buttons[i]) == HIGH) {
          delay(10);
        }
        delay(500);

        lightRandomLED();

      } else {
        //When wrong button is pressed
        digitalWrite(buzzer, HIGH);
        delay(150);
        digitalWrite(buzzer, LOW);

        Serial.println("Wrong button! Game reset.");
        if (bestReactionTime > 0) {
          Serial.print("Your overall best time was: ");
          Serial.print(bestReactionTime);
          Serial.println(" ms");
        }
        Serial.println("---------------------------");


        while (digitalRead(buttons[i]) == HIGH) {
          delay(10);
        }
        delay(500);


        digitalWrite(leds[currentLED], LOW);
        currentLED = -1;
        
        delay(1000); // 1 second penalty pause
        lightRandomLED();
      }

      break; 
    }
  }
}
