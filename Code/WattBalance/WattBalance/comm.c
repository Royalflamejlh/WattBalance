/*
 * Incoming Commands (To Atmega)
 */
typedef enum {
    REQ_PARAM, // The host computer is requesting to know the set paramaters.
    RESET, // Command to reset the balance.
    UPDATE_V_PERIOD, // Following commands update respective params.
    UPDATE_DAC_PERIOD,
    UPDATE_V_MIN,
    UPDATE_V_CALIBRATE_SAMPLES,
    UPDATE_V_MAX_SAMPLES,
    UPDATE_POS_ERROR,
    UPDATE_F_PERIOD,
    UPDATE_G,
    UPDATE_CYCLE_PARAM,
    UPDATE_MASS_OFFSET
} IncomingCommands;


void send_parameters(const ParameterData *params){
    
}

void send_sensors(const SensorData *sensors){
    
}



