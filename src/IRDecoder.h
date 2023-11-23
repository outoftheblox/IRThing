#ifndef IRDecoder_H
#define IRDecoder_H

#include <Arduino.h>

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

class IRDecoder
{
private:
    IRrecv& irRecv;//(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
    decode_results results;

public:
    bool resultsAvailable = false;

    IRDecoder(IRrecv& irRecv)
    :
        irRecv(irRecv)
    {};

    ~IRDecoder()
    {};

    void handle();
    void printInfo();
    void dumpACInfo();
    String decode();

};

#endif