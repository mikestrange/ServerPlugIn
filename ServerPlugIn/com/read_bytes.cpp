#include "read_bytes.h"


void ReadBytes::ReadObject(IReader& target)
{
    target.ReadFor(*this);
}