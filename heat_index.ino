#include <LiquidCrystal.h>
#include <math.h>
#include <DHT.h>
#include <string.h>

#define DHT_POWER 22

LiquidCrystal lcd(19, 18, 5, 17, 16, 4, 0);
DHT dht(23, DHT22);

float calculateHeatIndex(float celsius, float humidity) {
    float h = humidity;
    float f = (celsius * 9 / 5) + 32; // to farenheit
    float heatIndex = -42.379f + (2.04901523f * f) + (10.14333127f * h)
                      - (0.22475541f * f * h) - (6.83783e-3f * powf(f, 2))
                      - (5.481717e-2f * powf(h, 2)) + (1.22874e-3f * powf(f, 2) * h)
                      + (8.5282e-4f * f * powf(h, 2)) - (1.99e-6f * powf(f, 2) * powf(h, 2));
    heatIndex = (heatIndex - 32) * 5 / 9; // to celsius
    return heatIndex;
}

String noaaHeatEffect(float heatIndex)
{
  if (heatIndex >= 27 && heatIndex < 33) {
    return "MILD";
  }
  else if (heatIndex >= 33 && heatIndex < 40) {
    return "DISTRESS";
  }
  else if (heatIndex >= 40 && heatIndex < 52) {
    return "DANGER";
  }
  else if (heatIndex >= 52) {
    return "UNBEARABLE";
  }
  else {
    return "-";
  }
}

int roundFloat(float f) {
  int i = (int)f;
  float fraction;
  if (f < 0) {
    fraction = f + i;
    fraction = fraction * -1;
  } else {
    fraction = f - i;
  }
  if (fraction >= 0.5) {
    return (int)ceil(f);
  } else {
    return (int)floor(f);
  }
}

void setup() {  
  lcd.begin(16, 2);
  lcd.print("starting...");

  dht.begin();
  pinMode(DHT_POWER, OUTPUT);
}

void loop() {
  lcd.clear();  

  digitalWrite(DHT_POWER, HIGH);
  delay(2500);

  float celsius = dht.readTemperature();
  float humidity = dht.readHumidity();

  digitalWrite(DHT_POWER, LOW);

  if (isnan(celsius) || isnan(humidity)) {
    lcd.print("READ FAIL");
    delay(5000);
    return;
  }

  float heat = calculateHeatIndex(celsius, humidity);

  lcd.print("Feels: ");
  lcd.print(roundFloat(heat));
  lcd.print((char)223); // °
  lcd.print("C");
  lcd.print(" ");
  lcd.print(roundFloat(humidity));
  lcd.print((char)37); // %
  
  lcd.setCursor(0, 1);
  lcd.print("Risk: ");
  lcd.print(noaaHeatEffect(heat));

  delay(15000);
}
