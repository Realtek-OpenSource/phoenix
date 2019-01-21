#include "_ansi.h"
#include <stdfix.h>

/* For fixed-point type casting */

typedef union {
  char c[8];
  long l[2];
  long long ll;
  long long _Fract llr;
  long _Accum lk;
} type_switch;

/* Work around the lacked long long _Accum type */

typedef struct {
  long long hi;
  type_switch lo;
} long_long_Accum;

extern long_long_Accum sin_llk _PARAMS((long_long_Accum));
extern long_long_Accum cos_llk _PARAMS((long_long_Accum));
extern long_long_Accum atan_llk _PARAMS((long_long_Accum));
extern long_long_Accum log_llk _PARAMS((long_long_Accum));
extern long_long_Accum exp_llk _PARAMS((long_long_Accum));
extern long_long_Accum sqrt_llk _PARAMS((long_long_Accum));
extern long _Accum sin_lk _PARAMS((long _Accum));
extern long _Accum cos_lk _PARAMS((long _Accum));
extern long _Accum atan_lk _PARAMS((long _Accum));
extern long _Accum log_lk _PARAMS((long _Accum));
extern long _Accum exp_lk _PARAMS((long _Accum));
extern long _Accum sqrt_lk _PARAMS((long _Accum));

