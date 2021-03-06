#include <stdint.h>

#ifndef IGN_ANGLE_MG_BY_CYCLE
#define IGN_ANGLE_MG_BY_CYCLE


#define IGN_ANGLE_MG_BY_CYCLE_RPM_SCALE_N 16
#define IGN_ANGLE_MG_BY_CYCLE_MG_SCALE_N 16


static volatile float ign_angle_mg_by_cycle[IGN_ANGLE_MG_BY_CYCLE_MG_SCALE_N][IGN_ANGLE_MG_BY_CYCLE_RPM_SCALE_N] = {
    21.5,   22.5,   23.5,   25.0,   30.5,   31.5,   33.0,   37.0,   37.5,   38.0,   38.5,   39.5,   40.0,   41.0,   42.0,   42.0,   /*29*/
    21.5,   22.5,   23.5,   25.0,   30.5,   31.5,   33.0,   37.0,   37.5,   38.0,   38.5,   39.5,   40.0,   41.0,   42.0,   42.0,   /*57*/
    21.5,   22.5,   23.5,   25.0,   30.5,   31.5,   33.0,   37.0,   37.5,   38.0,   38.5,   39.5,   40.0,   41.0,   42.0,   42.0,   /*85*/
    21.5,   22.0,   23.0,   25.0,   30.5,   31.5,   33.0,   37.0,   37.5,   38.0,   38.5,   39.5,   40.0,   41.0,   42.0,   42.0,   /*113*/
    21.0,   22.0,   23.0,   25.0,   30.0,   31.0,   33.0,   36.0,   36.5,   37.0,   37.5,   38.5,   39.0,   41.0,   42.0,   42.0,   /*141*/
    19.0,   20.0,   21.0,   23.0,   29.0,   30.5,   32.5,   34.5,   35.0,   35.5,   36.0,   37.0,   38.0,   40.5,   41.5,   41.5,   /*169*/
    17.5,   18.5,   19.5,   21.5,   28.0,   29.5,   31.5,   33.5,   34.0,   34.5,   35.0,   35.5,   37.0,   39.0,   40.5,   40.5,   /*197*/
    16.5,   17.5,   18.5,   20.5,   26.5,   28.0,   30.5,   32.0,   32.5,   33.0,   34.0,   34.5,   36.5,   38.5,   39.0,   39.0,   /*225*/
    15.5,   16.5,   17.5,   19.5,   25.0,   27.0,   29.5,   30.5,   31.5,   32.0,   32.5,   33.0,   36.0,   36.5,   36.5,   36.5,   /*253*/
    12.5,   13.5,   14.5,   18.0,   24.0,   25.0,   28.5,   29.5,   30.0,   31.0,   31.5,   32.5,   35.0,   35.5,   35.5,   35.5,   /*281*/
    8.5,    9.5,    10.5,   14.5,   22.0,   23.5,   27.5,   29.0,   29.5,   30.5,   31.0,   32.0,   34.5,   35.0,   35.0,   35.0,   /*309*/
    6.0,    7.0,    8.0,    11.0,   19.5,   20.5,   26.0,   27.5,   28.5,   29.5,   30.5,   31.5,   34.0,   34.5,   34.5,   34.5,   /*337*/
    3.0,    4.0,    5.0,    8.5,    18.0,   19.0,   22.5,   24.0,   25.0,   27.0,   29.0,   30.0,   33.0,   34.0,   34.0,   34.0,   /*365*/
    2.5,    3.5,    4.5,    6.5,    14.0,   16.0,   21.0,   22.0,   23.0,   24.5,   27.5,   28.5,   31.5,   32.5,   32.5,   32.5,   /*393*/
    1.5,    2.5,    3.5,    5.5,    12.5,   15.0,   19.5,   20.5,   21.5,   23.0,   25.5,   26.5,   29.0,   30.0,   30.0,   30.0,   /*421*/
    0.5,    1.5,    2.5,    4.5,    12.0,   14.5,   18.5,   20.0,   21.0,   22.0,   24.0,   24.5,   26.0,   27.5,   28.0,   28.0    /*449*/
//  600     720     840     990     1170    1380    1560    1950    2310    2730    3210    3840    4530    5370    6360    7650    rpm\mg
};

static const uint16_t ign_angle_mg_by_cycle_rpm_scale[IGN_ANGLE_MG_BY_CYCLE_RPM_SCALE_N] = {
    600,    720,    840,    990,    1170,   1380,   1560,   1950,   2310,   2730,   3210,   3840,   4530,   5370,   6360,   7650
};

#define IGN_ANGLE_MG_BY_CYCLE_RPM_SCALE_MIN ign_angle_mg_by_cycle_rpm_scale[0]
#define IGN_ANGLE_MG_BY_CYCLE_RPM_SCALE_MAX ign_angle_mg_by_cycle_rpm_scale[IGN_ANGLE_MG_BY_CYCLE_RPM_SCALE_N-1]-1

static const uint16_t ign_angle_mg_by_cycle_mg_scale[IGN_ANGLE_MG_BY_CYCLE_MG_SCALE_N] = {
    29,     57,     85,     113,    141,    169,    197,    225,    253,    281,    309,    337,    365,    393,    421,    449
};

#define IGN_ANGLE_MG_BY_CYCLE_MG_SCALE_MIN ign_angle_mg_by_cycle_mg_scale[0]
#define IGN_ANGLE_MG_BY_CYCLE_MG_SCALE_MAX ign_angle_mg_by_cycle_mg_scale[IGN_ANGLE_MG_BY_CYCLE_MG_SCALE_N-1]-1

#endif /*IGN_ANGLE_MG_BY_CYCLE*/