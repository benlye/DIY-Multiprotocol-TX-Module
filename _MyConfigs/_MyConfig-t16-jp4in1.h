/*
   This file is meant to keep your settings after an upgrade of the multi source.
   If you know parameters you want for sure to be enabled or disabled in future
   then just force their values here.
   To enable  a setting use #define <setting name>
   To disable a setting use #undef  <setting name>
*/

// Forced CC2500 tuning for CC2500
#define FORCE_FRSKYD_TUNING -15
#define FORCE_FRSKYX_TUNING -15

// Disable PPM
#undef ENABLE_PPM

// Force the same global ID as other modules
#define FORCE_GLOBAL_ID 0x65428639
