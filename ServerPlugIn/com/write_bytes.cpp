#include "write_bytes.h"


void WriteBytes::WriteObject(IReader& target)
{
    target.WriteTo(*this);
}