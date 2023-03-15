#include <Wire.h>
#include <Adafruit_MCP23017.h>

// Define CD74HC4067 control pins
#define MUX_S0 0
#define MUX_S1 1
#define MUX_S2 2
#define MUX_SIG 3

// Define MCP23017 address and GPIO pins
#define MCP_ADDR 0x20
#define MCP_GPIO_START 0

// Create MCP23017 object
Adafruit_MCP23017 mcp;

void setup() {
  // Setup CD74HC4067 pins as outputs
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_SIG, INPUT);

  // Initialize I2C bus and MCP23017
  Wire.begin();
  mcp.begin(MCP_ADDR);

  // Setup MCP23017 GPIO pins as inputs
  for (int i = MCP_GPIO_START; i < MCP_GPIO_START + 16; i++) {
    mcp.pinMode(i, INPUT);
  }

  // Setup LED pin as output
  mcp.pinMode(16, OUTPUT);
}

void loop() {
  // Scan through each row of buttons
  for (int row = 0; row < 3; row++) {
    // Set CD74HC4067 control pins to select row
    digitalWrite(MUX_S0, row & 0x01);
    digitalWrite(MUX_S1, (row >> 1) & 0x01);
    digitalWrite(MUX_S2, (row >> 2) & 0x01);

    // Scan through each column of buttons in selected row
    for (int col = 0; col < 12; col++) {
      // Read button input from CD74HC4067 output pin
      int buttonState = digitalRead(MUX_SIG);

      // Calculate MCP23017 GPIO pin number
      int pinNum = (row * 12) + col;

      // Read current state of MCP23017 GPIO pin
      int currentPinState = mcp.digitalRead(MCP_GPIO_START + pinNum);

      // If button is pressed and pin is not already high, set pin high and turn on LED
      if (buttonState == HIGH && currentPinState == LOW) {
        mcp.digitalWrite(16, HIGH);
        mcp.digitalWrite(MCP_GPIO_START + pinNum, HIGH);
      }
      // If button is not pressed and pin is high, set pin low and turn off LED
      else if (buttonState == LOW && currentPinState == HIGH) {
        mcp.digitalWrite(16, LOW);
        mcp.digitalWrite(MCP_GPIO_START + pinNum, LOW);
      }
    }
  }
}
