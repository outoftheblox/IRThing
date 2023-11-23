#ifndef StringIterator_H
#define StringIterator_H

#include <Arduino.h>

class StringIterator
{
private:
    String& str;
    char seperator;
    uint8_t index;

public:
    StringIterator(String& str, char seperator)
    :
        str(str),
        seperator(seperator),
        index(0)
    {};

    ~StringIterator()
    {};

    String next();
    bool hasNext();
};

#endif