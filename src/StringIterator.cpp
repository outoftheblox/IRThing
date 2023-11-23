#include "StringIterator.h"

String StringIterator::next()
{
    int current = str.indexOf(seperator, index);
    if (current == -1)
    {
        String ret = str.substring(index);
        index = current + 1;
        return ret;
    }
    String ret = str.substring(index, current);
    index = current + 1;
    return ret;
}

bool StringIterator::hasNext()
{
    int current = str.indexOf(seperator, index);
    return current != -1;
}
