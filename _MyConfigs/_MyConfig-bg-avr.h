/*
   This file is meant to keep your settings after an upgrade of the multi source.
   If you know parameters you want for sure to be enabled or disabled in future
   then just force their values here.
   To enable  a setting use #define <setting name>
   To disable a setting use #undef  <setting name>
*/

// Disable invert telemetry
#undef INVERT_TELEMETRY

// Disable PPM
#undef ENABLE_PPM

// Force the Atmega module ID to be the same as the STM32 module
#define FORCE_GLOBAL_ID 0x65428639

// Configure tuning values for my Atmega module
#define FORCE_FRSKYD_TUNING -51
#define FORCE_FRSKYX_TUNING -51

// Disable unused A7105 protocols
#undef AFHDS2A_A7105_INO
#undef BUGS_A7105_INO
#undef HUBSAN_A7105_INO

// Disable unused CYRF6936 protocols
#undef DEVO_CYRF6936_INO
#undef DSM_CYRF6936_INO
#undef J6PRO_CYRF6936_INO
#undef WFLY_CYRF6936_INO
#undef WK2x01_CYRF6936_INO

// Disable unused CC2500 protocols
#undef FRSKYV_CC2500_INO

// Disable unused NRF24L01 protocols
#undef ASSAN_NRF24L01_INO
#undef CABELL_NRF24L01_INO
#undef CFLIE_NRF24L01_INO
#undef CG023_NRF24L01_INO
#undef CX10_NRF24L01_INO
#undef DM002_NRF24L01_INO
#undef ESKY_NRF24L01_INO
#undef ESKY150_NRF24L01_INO
#undef FQ777_NRF24L01_INO
#undef FY326_NRF24L01_INO
#undef GW008_NRF24L01_INO
#undef HONTAI_NRF24L01_INO
#undef HISKY_NRF24L01_INO
#undef KN_NRF24L01_INO
#undef MT99XX_NRF24L01_INO
#undef Q303_NRF24L01_INO
#undef SHENQI_NRF24L01_INO
#undef SLT_NRF24L01_INO
#undef SYMAX_NRF24L01_INO
#undef V2X2_NRF24L01_INO
#undef YD717_NRF24L01_INO

// Disable unused telemetry
#undef DSM_TELEMETRY
#undef AFHDS2A_FW_TELEMETRY
#undef AFHDS2A_HUB_TELEMETRY
#undef BAYANG_HUB_TELEMETRY
#undef BUGS_HUB_TELEMETRY
#undef HUBSAN_HUB_TELEMETRY
#undef CABELL_HUB_TELEMETRY
#undef HITEC_HUB_TELEMETRY
#undef HITEC_FW_TELEMETRY
