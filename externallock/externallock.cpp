#include <stdio.h>

#include "analograwdata.h"

int main(int argc, char *argv[])
{
    AnalogRawDataThreadSafe oneanalog;

	oneanalog.SetValue(1.0f);

    return 0;
}
