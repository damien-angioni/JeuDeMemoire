#include <LiquidCrystal.h>

LiquidCrystal screen(7, 8, 9, 10, 11, 12);

const byte BUTTONS[5] = {2, 3, 4, 5, 6};
const byte LEDS[5] = {A0, A1, A2, A3, A4};
const byte BUZZER = 13;

int points = 0;
int level = 1;

const byte MAX_SEQUENCE = 24;
byte sequence[MAX_SEQUENCE] = {};
byte sequence_index = 0;

int button_pressed_id = -1;
int prev_button_pressed_id = -1;


void create_sequence() {
  for (int i = 0; i < MAX_SEQUENCE; i++) {
    if (i < level+2) {
      byte num = random(2, 7);    //random number between 2 and 6 (7 excluded)
      sequence[i] = num;
    } else {
      sequence[i] = 0;
    }
  }
}

void play_demo() {
  button_pressed_id = -1;
  prev_button_pressed_id = -1;
  for (int i = 0; i < level+2; i++) {
    if (i >= MAX_SEQUENCE) { break; }

    digitalWrite(sequence[i]+12, HIGH);
    delay(500);

    digitalWrite(sequence[i]+12, LOW);
    delay(100);
  }
}

bool check_input(int button) {
  if (BUTTONS[button] == sequence[sequence_index]) {
    return true;
  }
  return false;
}

void input(int button) {
  if (check_input(button) == true) {
      sequence_index++;
    } else {
      screen.setCursor(10, 0);
      screen.write("Perdu!");
      digitalWrite(BUZZER, HIGH);
      delay(500);
      digitalWrite(BUZZER, LOW);
      delay(1500);
      reset(1);
    }
  
}

void blit_screen() {
  screen.clear();
  screen.setCursor(0, 0);
  screen.write("Niveau");
  screen.setCursor(7, 0);
  screen.print(level);
  screen.setCursor(0, 1);
  screen.write("Points");
  screen.setCursor(7, 1);
  screen.print(points);
}

void setup() {
  randomSeed(analogRead(A5));

  for (int i = 0; i < 5; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(LEDS[i], OUTPUT);
  }

  screen.begin(16, 2);
  screen.clear();

  reset(1);
}

void loop() {
  button_pressed_id = -1;
  for (int i = 0; i < 5; i++) {
    if (digitalRead(BUTTONS[i]) == HIGH) {
      button_pressed_id = i;
    }
  }
  if (sequence_index >= level+2) {                      //If sequence is correct
    screen.setCursor(10, 0);
    screen.write("Bravo!");
    delay(2000);
    reset(level+1);
  } else if (button_pressed_id >= 0 && prev_button_pressed_id == -1) {    //Button input
    input(button_pressed_id);
  }
    
  prev_button_pressed_id = button_pressed_id;

  //Lights
  for (int i = 0; i < 5; i++) {
      if (digitalRead(BUTTONS[i]) == HIGH) {
        digitalWrite(LEDS[i], HIGH);
      } else {
        digitalWrite(LEDS[i], LOW);
      }
    }
  delay(1);
}

void reset(int level_id) {
  blit_screen();
  level = level_id;
  if (level <= 1) {
    level = 1;
    points = 0;
  } else {
    points++;
  }
  button_pressed_id = -1;
  prev_button_pressed_id = -1;
  sequence_index = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(LEDS[i], LOW);
  }
  delay(400);
  blit_screen();
  create_sequence();
  play_demo();
}
