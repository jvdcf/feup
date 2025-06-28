#include "papi.hpp"
#include <cstddef>
#include <iostream>

PAPI::PAPI() {
    #ifdef PAPI_ENABLE
    int ret;
    ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		std::cout << "FAIL" << std::endl;

	ret = PAPI_create_eventset(&eventSet);
	if (ret != PAPI_OK) std::cout << "ERROR: create eventset" << std::endl;

	ret = PAPI_add_event(eventSet, PAPI_L1_DCM);
	if (ret != PAPI_OK) std::cout << "ERROR: PAPI_L1_DCM" << std::endl;

	ret = PAPI_add_event(eventSet, PAPI_L2_DCM);
	if (ret != PAPI_OK) std::cout << "ERROR: PAPI_L2_DCM" << std::endl;

    ret = PAPI_add_event(eventSet, PAPI_L2_DCA);
    if (ret != PAPI_OK) std::cout << "ERROR: PAPI_L2_DCA" << std::endl;

    ret = PAPI_add_event(eventSet, PAPI_DP_OPS);
    if (ret != PAPI_OK) std::cout << "ERROR: PAPI_DP_OPS" << std::endl;
    #else
    std::cout << "WARN: PAPI is disabled! Define PAPI_ENABLE in papi.hpp to enable it." << std::endl;
    #endif
}

void PAPI::start() {
    #ifdef PAPI_ENABLE
	if (PAPI_start(eventSet) != PAPI_OK) {
        std::cout << "ERROR: Start PAPI" << std::endl;
    }
    #endif
}

void PAPI::stop() {
    #ifdef PAPI_ENABLE
    if (PAPI_stop(eventSet, values) != PAPI_OK) {
        std::cout << "ERROR: Stop PAPI" << std::endl;
    }
    #endif
}

void PAPI::print(double duration) {
    #ifdef PAPI_ENABLE
    std::cout << "MFLOPS: " << values[3] / duration / 1e6 << std::endl;
    std::cout << "% L2 Hits: " << 1.0 - ((double)values[1] / values[2]) << std::endl;
    std::cout << "L1 DCM: " << values[0] << std::endl;
    std::cout << "L2 DCM: " << values[1] << std::endl;
    std::cout << "L2 DCA: " << values[2] << std::endl;
    std::cout << "DP OPS: " << values[3] << std::endl;
    #endif
    (void)duration;
}

void PAPI::to_csv(double duration, size_t dim, size_t bs) {
// size, time, l1DCM, l2DCM, l2DCA, l2hitpercent, dpops, mflops
#ifdef PAPI_ENABLE
    std::cout
    << dim << ","
    << duration << ",";
    if (bs != 0) std::cout << bs << ",";
    std::cout
    << values[0] << ","
    << values[1] << ","
    << values[2] << ","
    << 1.0 - ((double)values[1] / values[2]) << ","
    << values[3] << ","
    << values[3] / duration / 1e6 << std::endl;    
#endif
    (void)duration;
    (void)dim;
    (void)bs;
}

void PAPI::reset() {
    #ifdef PAPI_ENABLE
    if (PAPI_reset(eventSet) != PAPI_OK) {
        std::cout << "FAIL reset" << std::endl; 
    }
    #endif
}

PAPI::~PAPI() {
    #ifdef PAPI_ENABLE
    int ret;

    ret = PAPI_remove_event(eventSet, PAPI_L1_DCM);
	if (ret != PAPI_OK) std::cout << "FAIL remove event" << std::endl; 

	ret = PAPI_remove_event(eventSet, PAPI_L2_DCM);
	if (ret != PAPI_OK) std::cout << "FAIL remove event" << std::endl; 

    ret = PAPI_remove_event(eventSet, PAPI_L2_DCA);
    if (ret != PAPI_OK) std::cout << "FAIL remove event" << std::endl;

    ret = PAPI_remove_event(eventSet, PAPI_DP_OPS);
    if (ret != PAPI_OK) std::cout << "FAIL remove event" << std::endl;

	ret = PAPI_destroy_eventset(&eventSet);
	if (ret != PAPI_OK) std::cout << "FAIL destroy" << std::endl;
    #endif
}
