#include <stdio.h>
#include <stdlib.h>

#include "mminfo.h"

int main(void) {
IGNORE_WARNING_PUSH("-Wpointer-arith")
    MmTest item = {
        .NfovThermoTable = {
            {1.279, 1.140, 1.000, 0.860, 0.720, 0.581, 0.441, 0.301, 0.161, 0.022, -0.117, -0.257, -0.327, -0.397, -0.466},
            {2.179, 2.040, 1.900, 1.760, 1.620, 1.481, 1.341, 1.201, 1.061, 0.922, 0.782, 0.642, 0.572, 0.502, 0.433},
        },
        .WfovThermoTable = {
            {-2.470, -2.110, -1.750, -1.390, -1.029, -0.669, -0.309, 0.050, 0.410, 0.770, 1.130, 1.490, 1.670, 1.850, 2.031},
            {-3.020, -2.660, -2.300, -1.940, -1.579, -1.219, -0.859, -0.499, -0.139, 0.220, 0.580, 0.940, 1.120, 1.300, 1.481},
        },
        .NfovBasePosition = 8.250,
        .WfovBasePosition = 43.500,
        .CalibrationPosition = 13.0,
        .FocusSpeedMax = 0.3,
        .FocusSpeedMin = 0.075,
        .FOVChangeTimeMax = 4.4,
        .BeginChangeFocusSpeedTime = 1.0,
        .EndChangeFocusSpeedTime = 3.0,
        .StatePollingTime = 15,
        .ExchangeErrorNumber = 3,
        .TemperatureDelta = 0.5,
        .Fruit = FR_TOMATO,
        .Command = CMD_READ_LINE_THERMO,
        .is_cool = true,
        .timer = 12651837659,
        .ptrdiff = (void *)0xDEADBEEF - NULL,
        .sptr = (ptr_t)-1,
        .req = {
            .cmd = CMD_ERROR,
            .fruit = FR_PEAR,
        }
    };
IGNORE_WARNING_POP()

    char buf[1024 * 2];
    if(sprint_MmTest(buf, item, 0, "item") > 0) fputs(buf, stdout);
    else printf("Cannot print\n");

    return 0;
}

int sprint_ThermoTable(char *stream, ThermoTable table, int lvl, const char *prefix) {
    int i, j;
    bool first = true;
    char *end_stream = stream;
    end_stream += sprintf(end_stream, "%*s%s: \n", lvl, "", prefix);
    for(i = 0; i < THERMO_TABLE_ROWS; i++) {
        end_stream += sprintf(end_stream, "%*s[", lvl+4, "");
        for(j = 0; j < THERMO_TABLE_COLUMNS; j++) {
            if(!first) {
                end_stream += sprintf(end_stream, ", ");
            } else {
                first = false;
            }
            end_stream += sprintf(end_stream, "%+-5.3f", table[i][j]);
        }
        first = true;
        end_stream += sprintf(end_stream, "]\n");
    }
    return end_stream - stream;
}
