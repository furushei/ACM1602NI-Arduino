# Arduino ACM1602NI library
[ACM1602NI-FLW-FBW](http://akizukidenshi.com/catalog/g/gP-05693/) is a 16x2 character LCD module with I2C interface. This library allow Arduino boards to control them, and is compatible with [the Arduino LiquidCrystal library](https://github.com/arduino-libraries/LiquidCrystal).

## Installation
1. Download ZIP from GitHub
2. Arduino IDE -> Scketch menu -> Include library -> Add .ZIP library...

## Usage
This library is compatible with LiquidCrystal library, so you can run the examples for LiquidCrystal library with only few changes:
 - Change `#include <LiquidCrystal.h>` to `#include <ACM1602NI.h>`
 - Change `LiquidCrystal lcd(...);` to `ACM1602NI lcd;`

Note: this library calls `Wire.begin` in `ACM1602NI::begin` method, so you don't need to call.

Here is a simple "Hello, world!" example:
```c++
#include <ACM1602NI.h>

ACM1602NI lcd;
boolean isFirst = true;

void setup() {
  lcd.begin();
}

void loop() {
  if (isFirst) {
    lcd.print("Hello, world!");
    isFirst = false;
  }
}
```

## License
LGPL
