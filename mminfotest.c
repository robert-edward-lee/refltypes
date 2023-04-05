#include <stdbool.h>
#include <stdio.h>

#include "mminfo.h"
 
static const char *const fr_str[] = {DECLARE_FRUITS(ENUM_EXPANDER_STR)};
static const char *const cmd_str[] = {DECLARE_COMMANDS(ENUM_EXPANDER_STR_DESIGNATED)};

int main(void) {
    MMInfoStruct item = {
        .NFOVTermotable = {
            {1.279, 1.140, 1.000, 0.860, 0.720, 0.581, 0.441, 0.301, 0.161, 0.022, -0.117, -0.257, -0.327, -0.397, -0.466},
            {2.179, 2.040, 1.900, 1.760, 1.620, 1.481, 1.341, 1.201, 1.061, 0.922, 0.782, 0.642, 0.572, 0.502, 0.433},
        },
        .WFOVTermotable = {
            {-2.470, -2.110, -1.750, -1.390, -1.029, -0.669, -0.309, 0.050, 0.410, 0.770, 1.130, 1.490, 1.670, 1.850, 2.031},
            {-3.020, -2.660, -2.300, -1.940, -1.579, -1.219, -0.859, -0.499, -0.139, 0.220, 0.580, 0.940, 1.120, 1.300, 1.481},
        },
        .NFOVBasePosition = 8.250,
        .WFOVBasePosition = 43.500,
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
    };

    char buf[1024];
    if(sprint_mm(buf, &item) > 0) fputs(buf, stdout);


    Request req = {
        .fruit = FR_BANANA,
        .cmd = CMD_DRIVE_LINE,
    };

    printf("\n\n\n");

    if(sprint_request(buf, &req) > 0) fputs(buf, stdout);

    return 0;
}

int sprint_mm(char *stream, const MMInfoStruct *s) {
    char *end_stream = stream;
IGNORE_WARNING_PUSH("-Waddress-of-packed-member")
    DECLARE_MM_INFO(STRUCT_EXPANDER_AS_PRINTER)
IGNORE_WARNING_POP()
    return end_stream - stream;
}

int sprint_Termotable(char *stream, const Termotable table) {
    int i, j;
    bool first = true;
    char *end_stream = stream;
    end_stream += sprintf(end_stream, "{\n");
    for(i = 0; i < TERMOTABLE_ROW_COUNT; i++) {
        end_stream += sprintf(end_stream, "\t{");
        for(j = 0; j < TERMOTABLE_COLUMN_COUNT; j++) {
            if(!first) {
                end_stream += sprintf(end_stream, ", ");
            } else {
                first = false;
            }
            end_stream += sprintf(end_stream, "%+-5.3f", table[i][j]);
        }
        first = true;
        end_stream += sprintf(end_stream, "},\n");
    }
    end_stream += sprintf(end_stream, "}");
    return end_stream - stream;
}

int sprint_Fruits(char *stream, Fruits fruit) {
    return sprintf(stream, "%s", fr_str[fruit]);
}

int sprint_Commands(char *stream, Commands cmd) {
    return sprintf(stream, "%s", cmd_str[(size_t)cmd]);
}

const char *stringify_fruit(Fruits fruit) {
    return fr_str[fruit];
}

const char *stringify_cmd(Commands cmd) {
    return cmd_str[(size_t)cmd];
}

int sprint_request(char *stream, const Request *s) {
    char *end_stream = stream;
    DECLARE_REQUEST(BITFIELDS_EXPANDER_AS_PRINTER)
    return end_stream - stream;
}