/*
   This file is meant to keep your settings after an upgrade of the multi source.
   If you know parameters you want for sure to be enabled or disabled in future
   then just force their values here.
   To enable  a setting use #define <setting name>
   To disable a setting use #undef  <setting name>
*/

// Forced CC2500 tuning for CC2500
#define FORCE_FRSKYD_TUNING 23
#define FORCE_FRSKYX_TUNING 23

// Configure MULTI telemetry for erSkyTx
#undef INVERT_TELEMETRY
#undef MULTI_TELEMETRY
#define MULTI_STATUS

// Disable PPM
#undef ENABLE_PPM
