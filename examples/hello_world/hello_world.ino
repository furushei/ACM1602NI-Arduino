#include <Wire.h>
#include <ACM1602NI.h>

ACM1602NI lcd;
boolean isFirst = true;

void setup() {
  Wire.begin();
  lcd.begin();
}

void loop() {
  if (isFirst) {
    lcd.print("Hello,");
    lcd.setCursor(4, 1);
    lcd.print("World!");
    isFirst = false;
  }
}
