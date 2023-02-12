#include <StaticSerialCommands.h>
#include <FastLED.h>

#define MIN_LIGHTS 1
#define MAX_LIGHTS 40
#define LED_PIN     3
#define NUM_LEDS    30


int LightsNum = 1; // number of LEDs in array, 1 by default

CRGB leds[NUM_LEDS];

void cmd_help(SerialCommands& sender, Args& args) {
  sender.listCommands();
}

void set(SerialCommands& sender, Args& args) {
  LightsNum = args[0].getInt();
  sender.getSerial().print(F("Amount of lights set to: "));
  sender.getSerial().println(LightsNum);
}

void turn(SerialCommands& sender, Args& args) {
  int LightsIdx = args[0].getInt();
  String LedPos = args[1].getString();
  String LedColor = args[2].getString();

  int idx = 0;

  if(LedPos == "up"){
    idx = LightsIdx*2;
  }
  else if(LedPos == "down"){
    idx = LightsIdx*2 + 1;
  }

  //leds[idx] = CRGB::LedColor; FastLED.show(); delay(100);
  //leds[idx] = CRGB::Black; FastLED.show(); delay(100);

  if(LedColor == "red"){
    leds[idx] = CRGB::Red;FastLED.show();
  }
  else if(LedColor == "green"){
    leds[idx] = CRGB::Green;FastLED.show();
  }
  else if(LedColor == "yellow"){
    leds[idx] = CRGB::Yellow;FastLED.show();
  }
  else{
    leds[idx] = CRGB::Purple;FastLED.show();
  }

  
  sender.getSerial().print(F("Turn light "));
  sender.getSerial().print(LightsIdx);
  sender.getSerial().print(F("  "));
  sender.getSerial().print(LedPos);
  sender.getSerial().print(F("  "));
  sender.getSerial().println(LedColor);

}

void turn_off(SerialCommands& sender, Args& args) {
  int LightsIdx = args[0].getInt();
  String LedPos = args[1].getString();

  int idx = 0;

  if(LedPos == "up"){
    idx = LightsIdx*2;
  }
  else if(LedPos == "down"){
    idx = LightsIdx*2 + 1;
  }
  leds[idx] = CRGB::Black;FastLED.show();

  sender.getSerial().print(F("Turn off  light "));
  sender.getSerial().print(LightsIdx);
  sender.getSerial().print(F(" pos "));
  sender.getSerial().println(LedPos);
}


/*
COMMAND macro is used to create Command object.
It takes the following arguments:
    COMMAND(function, command)
    COMMAND(function, command, subcommands)
    COMMAND(function, command, subcommands, description)
    COMMAND(function, command, arguments..., subcommands, description)

ARG macro is used to specify argument type, range (if type is numeric) and name.
It takes the following arguments:
    ARG(type)
    ARG(type, name)
    ARG(type, min, max)
    ARG(type, min, max, name)
*/

Command commands[] {
  COMMAND(cmd_help, "help", NULL, "list commands"),

  COMMAND(set, "set", ARG(ArgType::Int, MIN_LIGHTS, MAX_LIGHTS, "LightsNum"), NULL, "set the number of lights in array"),
  COMMAND(turn, "turn", ArgType::Int, ArgType::String,ArgType::String, NULL, "turn on specific color LED"),
  COMMAND(turn_off, "off", ArgType::Int, ArgType::String, NULL, "turn off specific LED"),
};

SerialCommands serialCommands(Serial, commands, sizeof(commands) / sizeof(Command));

// if default buffer size (64) is too small pass a buffer through constructor
// char buffer[128];
// SerialCommands serialCommands(Serial, commands, sizeof(commands) / sizeof(Command), buffer, sizeof(buffer));

void setup() {
  //delay(30); // 3 second delay for recovery

  Serial.begin(9600);

  //FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);

  serialCommands.listCommands();
}

void loop() {
  serialCommands.readSerial();
}
