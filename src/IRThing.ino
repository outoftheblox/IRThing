#include <Arduino.h>
#include <Thing.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <BlinkPattern.h>
#include "StringIterator.h"
#include "IRDecoder.h"

#include <IRutils.h>
#if DECODE_AC
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Kelvinator.h>
#include <ir_Midea.h>
#include <ir_Toshiba.h>
#endif

using namespace ootb;

#define IR_LED D3
#define RECV_PIN D4
#define CAPTURE_BUFFER_SIZE 1024
#define TIMEOUT 50U

#define MIN_UNKNOWN_SIZE 12

Thing thing;
BlinkPattern led(BUILTIN_LED);
IRsend irsend(IR_LED);
IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
IRDecoder decoder(irrecv);

BlinkPattern::Pattern<2> start{{1, 9}, 25};
BlinkPattern::Pattern<0> disable{{}, 1000};

String sensorTopic = "esp/" + thing.clientId() + "/ir/receive";
String sendTopic = "esp/" + thing.clientId() + "/ir/send";

int hexStringToInt(const char str[])
{
    return (int)strtol(str, 0, 16);
}

int decStringToInt(const char str[])
{
    return (int)strtol(str, 0, 10);
}

void setup()
{
    irsend.begin();

    irrecv.setUnknownThreshold(MIN_UNKNOWN_SIZE);
    irrecv.enableIRIn(); // Start the receiver

    Serial.begin(230400);
    Serial.println();
    Serial.println("Client:" + thing.clientId());

    led.setPattern(start);

    thing.onStateChange([](const String &msg)
    {
        Serial.println(msg);
    });

    thing.begin();
    thing.addActuator(sendTopic, [](Value &value)
    {
        String line = value;
        StringIterator it = StringIterator(line, ' ');
        int protocol = decStringToInt(it.next().c_str());
        int data = hexStringToInt(it.next().c_str());
        int len = decStringToInt(it.next().c_str());
        int rep = decStringToInt(it.next().c_str());
        Serial.print(protocol);
        Serial.print(" ");
        Serial.print(data, HEX);
        Serial.print(" ");
        Serial.print(len);
        Serial.print(" ");
        Serial.print(rep);
        Serial.println();
        irsend.send(static_cast<decode_type_t>(protocol), data, len, rep);
        digitalWrite(BUILTIN_LED, 0);
    });
    led.setPattern(disable);
}

void loop()
{
    thing.handle();
    led.handle();
    decoder.handle();
    if (decoder.resultsAvailable)
    {
        decoder.printInfo();
        String line = decoder.decode();
        Serial.println(line);
        Value v = line;
        thing.publish(sensorTopic, v);
    }
}
