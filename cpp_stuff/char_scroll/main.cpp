#include <EEPROM.h>
#include "letter.h"

int row[] = {23, 25, 27, 29, 31, 33, 35, 37}; //Anode
int col[] = {39, 41, 43, 45, 47, 49, 51, 53}; //Cathode

int button_time = 0;
int button_count = 0;
const int dir_button = 2;
int direction = -1;
int speed = 5;

void write_character(const int letter[8], int offset)
{
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      digitalWrite(col[j], !bitRead(letter[i], 7-j+offset));
    }
    digitalWrite(row[i], HIGH);
    delay(speed);
    digitalWrite(row[i], LOW);
  }
}

void scroll_text(String s)
{
  for (int i = 0; i < s.length(); i++) {
    int offset = -7 * direction;
    while (offset <= 7 && offset >= -10) {
      write_character(font[s.charAt(i)], offset);
      offset += direction;
    }
    if (Serial.available() > 0 || button_count > 0) {
      //stop current text if new text is waiting
      break;
    }
  }
}

String load_text()
{
  String saved;
  for (int i = 0; EEPROM[i] != '\0'; i++) {
    //load string by character
    saved += (char)EEPROM[i];
  }
  return saved;
}

void save_text(String s)
{
  int i;
  for (i = 0; i < s.length(); i++) {
    //save string by character
    EEPROM[i] = s.charAt(i);
  }
  //add null terminator
  EEPROM[i] = '\0';
}

void handle_button() {
  switch(button_count) {
    case 1:
      direction = -direction;
      break;
    case 2:
      if (speed == 5) {
        speed = 10;
      } else if (speed == 10) {
        speed = 15;
      } else {
        speed = 5;
      }
      break;
  }
}

void button_ISR()
{
  button_time = millis();
  if (button_count == 0 || millis() - button_time < 500) {
    button_count++;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Enter text you would like to display:");
  //set all pins to outputs
  for(int i = 0; i < 8; i++) {
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }

  pinMode(dir_button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(dir_button), button_ISR, FALLING);

  save_text(String("Eamon"));
}

void loop()
{
  String display_text = load_text();

  if (Serial.available() > 0) {
    display_text = Serial.readStringUntil('\n');
    save_text(display_text);
  }
  if (millis() - button_time > 500) {
    handle_button();
    button_count = 0;
  }

  scroll_text(display_text);
}
