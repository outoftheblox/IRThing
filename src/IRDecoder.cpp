#include "IRDecoder.h"

// Display the human readable state of an A/C message if we can.
void IRDecoder::dumpACInfo()
{
    decode_results* results = &(this->results);
    String description = "";
#if DECODE_DAIKIN
    if (results->decode_type == DAIKIN)
    {
        IRDaikinESP ac(0);
        ac.setRaw(results->state);
        description = ac.toString();
    }
#endif // DECODE_DAIKIN
#if DECODE_FUJITSU_AC
    if (results->decode_type == FUJITSU_AC)
    {
        IRFujitsuAC ac(0);
        ac.setRaw(results->state, results->bits / 8);
        description = ac.toString();
    }
#endif // DECODE_FUJITSU_AC
#if DECODE_KELVINATOR
    if (results->decode_type == KELVINATOR)
    {
        IRKelvinatorAC ac(0);
        ac.setRaw(results->state);
        description = ac.toString();
    }
#endif // DECODE_KELVINATOR
#if DECODE_TOSHIBA_AC
    if (results->decode_type == TOSHIBA_AC)
    {
        IRToshibaAC ac(0);
        ac.setRaw(results->state);
        description = ac.toString();
    }
#endif // DECODE_TOSHIBA_AC
#if DECODE_GREE
    if (results->decode_type == GREE)
    {
        IRGreeAC ac(0);
        ac.setRaw(results->state);
        description = ac.toString();
    }
#endif // DECODE_GREE
#if DECODE_MIDEA
    if (results->decode_type == MIDEA)
    {
        IRMideaAC ac(0);
        ac.setRaw(results->value); // Midea uses value instead of state.
        description = ac.toString();
    }
#endif // DECODE_MIDEA
#if DECODE_HAIER_AC
    if (results->decode_type == HAIER_AC)
    {
        IRHaierAC ac(0);
        ac.setRaw(results->state);
        description = ac.toString();
    }
#endif // DECODE_HAIER_AC
    // If we got a human-readable description of the message, display it.
    if (description != "")
        Serial.println("Mesg Desc.: " + description);
}

void IRDecoder::handle()
{
    resultsAvailable = irRecv.decode(&results);
}

void IRDecoder::printInfo()
{
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf("Timestamp : %06u.%03u\n", now / 1000, now % 1000);
    if (results.overflow)
        Serial.printf("WARNING: IR code is too big for buffer (>= %d). "
                        "This result shouldn't be trusted until this is resolved. "
                        "Edit & increase CAPTURE_BUFFER_SIZE.\n"
                        );
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&results));
    dumpACInfo(); // Display any extra A/C info if we have it.
    yield();              // Feed the WDT as the text output can take a while to print.

    // Display the library version the message was captured with.
    Serial.print("Library   : v");
    Serial.println(_IRREMOTEESP8266_VERSION_);
    Serial.println();

    // Output RAW timing info of the result.
    Serial.println(resultToTimingInfo(&results));
    yield(); // Feed the WDT (again)

    // Output the results as source code
    Serial.println(resultToSourceCode(&results));
    Serial.println(""); // Blank line between entries
    yield();            // Feed the WDT (again)
}

String IRDecoder::decode()
{
    String decode_type = String(results.decode_type);
    String value = String(results.value, HEX);
    String bits = String(results.bits);
    String line = "" + decode_type + " " + value + " " + bits + " " + 2;
    return line;
}
