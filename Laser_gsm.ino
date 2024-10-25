#include <SoftwareSerial.h>

const String PHONE = "+919639475231";

#define rxPin 2
#define txPin 3
SoftwareSerial sim800L(rxPin, txPin);

#define LASER_RECEIVER_PIN 4
#define LASER_TRANSMIT_PIN 5
boolean laser_flag = 0;
#define BUZZER_PIN 8

void setup()
{
  Serial.begin(115200);
  sim800L.begin(9600);
  pinMode(LASER_RECEIVER_PIN, INPUT);
  pinMode(LASER_TRANSMIT_PIN, OUTPUT);
  digitalWrite(LASER_TRANSMIT_PIN, HIGH);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Initializing...");
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);
}

void loop()
{
  while (sim800L.available())
  {
    String response = sim800L.readString();
    Serial.println(response);

    if (response.indexOf("RING") != -1)
    {
      Serial.println("Incoming call detected.");
      ring_buzzer();
    }
  }

  int laser_value = digitalRead(LASER_RECEIVER_PIN);

  // The laser trip wire is detected, trigger Alarm and send SMS
  if (laser_value == HIGH)
  {
    if (laser_flag == 1)
    {
      Serial.println("Laser Tripwire Detected.");
      laser_flag = 0;
      make_call();
    }
    ring_buzzer();
  }
  // No laser tripwire is detected, turn OFF Alarm
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
    laser_flag = 1;
  }
}

void make_call()
{
  Serial.println("calling....");
  sim800L.println("ATD" + PHONE + ";");

  // Do not delay to allow the buzzer to keep ringing during the call
}

void ring_buzzer()
{
  // Police Siren Melody
  for (int i = 0; i < 2; i++) {
    // Upward pitch
    for (int frequency = 300; frequency <= 1200; frequency += 50) {
      tone(BUZZER_PIN, frequency, 10);
      delay(10);
    }

    delay(200); // Pause between upward and downward pitch

    // Downward pitch
    for (int frequency = 1200; frequency >= 300; frequency -= 50) {
      tone(BUZZER_PIN, frequency, 10);
      delay(10);
    }

    delay(200); // Pause between downward and upward pitch
  }

  // Stop the buzzer
  noTone(BUZZER_PIN);
}
