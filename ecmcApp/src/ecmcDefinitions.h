
/*
 * ESSOSMCU-definitions.h
 *
 *  Created on: March 14, 2016
 *      Author: anderssandstrom
 */

#ifndef ECMCDEFINITIONS_H_
#define ECMCDEFINITIONS_H_

#define UNPACK( ... ) __VA_ARGS__

#define MCU_FREQUENCY 1000.0
#define MCU_NSEC_PER_SEC 1000000000
#define MCU_PERIOD_NS (int)(MCU_NSEC_PER_SEC / MCU_FREQUENCY)
#define DIFF_NS(A, B) (((B).tv_sec - (A).tv_sec) * MCU_NSEC_PER_SEC + \
  (B).tv_nsec - (A).tv_nsec)
#define ECMC_MAX_AXES 8

//Test new conversion
#define TIMESPEC2NS(T) ((uint64_t) (((T).tv_sec - 946684800ULL) * 1000000000ULL) + (T).tv_nsec)
//#define TIMESPEC2NSEPOCH2000(T) ((uint64_t) (((T).tv_sec - 946684800ULL) * 1000000000ULL) + (T).tv_nsec)

//#define MSG_TICK 0
#define MAX_MESSAGE 10000

//Transforms
#define MAX_TRANSFORM_INPUTS ECMC_MAX_AXES*2
#define TRANSFORM_EXPR_LINE_END_CHAR '#'
#define TRANSFORM_EXPR_OUTPUT_VAR_NAME "out"
#define TRANSFORM_EXPR_COMMAND_EXECUTE_PREFIX "ex"
#define TRANSFORM_EXPR_COMMAND_ENABLE_PREFIX "en"
#define TRANSFORM_EXPR_VARIABLE_TRAJ_PREFIX "setPos"
#define TRANSFORM_EXPR_VARIABLE_ENC_PREFIX "actPos"
#define TRANSFORM_EXPR_INTERLOCK_PREFIX "ilock"

//EtherCAT
#define EC_MAX_PDOS 200        //Pdos per slave terminal
#define EC_MAX_ENTRIES 200     //Entries Per slave
#define EC_MAX_SLAVES 200

//Data recording
#define ECMC_MAX_DATA_RECORDERS_OBJECTS 10
#define ECMC_MAX_EVENT_OBJECTS 10
#define ECMC_MAX_DATA_STORAGE_OBJECTS 10
#define ECMC_DEFAULT_DATA_STORAGE_SIZE 1000
#define ECMC_MAX_EVENT_CONSUMERS 10
#define ECMC_MAX_COMMANDS_LISTS 10
#define ECMC_MAX_COMMANDS_IN_COMMANDS_LISTS 100

//Motion
enum app_mode_type{
  ECMC_MODE_CONFIG=0,
  ECMC_MODE_RUNTIME=1
};

enum {
  ECMC_PRIO_LOW = 0,
  ECMC_PRIO_HIGH = 60
};

enum axisType{
  ECMC_AXIS_TYPE_BASE=0,
  ECMC_AXIS_TYPE_REAL=1,
  ECMC_AXIS_TYPE_VIRTUAL=2,
  ECMC_AXIS_TYPE_TRAJECTORY=3,
  ECMC_AXIS_TYPE_ENCODER=4,
};

enum operationMode{
  ECMC_MODE_OP_AUTO=0,
  ECMC_MODE_OP_MAN=1,
};

enum motionCommandTypes {
  ECMC_CMD_NOCMD=-1,
  ECMC_CMD_JOG=0,
  ECMC_CMD_MOVEVEL=1,
  ECMC_CMD_MOVEREL=2,
  ECMC_CMD_MOVEABS=3,
  ECMC_CMD_MOVEMODULO=4,     //NOT IMPLEMENTED
  ECMC_CMD_HOMING=10,        //PARTLY IMPLEMENTED
  ECMC_CMD_SUPERIMP=20,      //NOT IMPLEMENTED
  ECMC_CMD_GEAR=30 ,         //NOT IMPLEMENTED (implemented in another way..) Should adjust code to enable gear only when nCommand=30
};

enum motionDirection {
  ECMC_DIR_FORWARD=0,
  ECMC_DIR_BACKWARD=1,
  ECMC_DIR_STANDSTILL=2,
};

enum motionMode{
  ECMC_MOVE_MODE_POS=0,
  ECMC_MOVE_MODE_VEL=1,
};

enum dataSource{
  ECMC_DATA_SOURCE_INTERNAL=0,
  ECMC_DATA_SOURCE_EXTERNALENCODER=1,
  ECMC_DATA_SOURCE_EXTERNALTRAJECTORY=2,
};

enum coordSystMode{
  ECMC_COORD_ABS=0,
  ECMC_COORD_REL=1,
};

enum stopMode{
  ECMC_STOP_MODE_EMERGENCY=0,
  ECMC_STOP_MODE_NORMAL=1,
  ECMC_STOP_MODE_RUN=2,
};

enum interlockTypes {
  ECMC_INTERLOCK_NONE=0,
  ECMC_INTERLOCK_SOFT_BWD=1,
  ECMC_INTERLOCK_SOFT_FWD=2,
  ECMC_INTERLOCK_HARD_BWD=3,
  ECMC_INTERLOCK_HARD_FWD=4,
  ECMC_INTERLOCK_NO_EXECUTE=5,
  ECMC_INTERLOCK_POSITION_LAG=6,
  ECMC_INTERLOCK_BOTH_LIMITS=7,
  ECMC_INTERLOCK_EXTERNAL=8,
  ECMC_INTERLOCK_TRANSFORM=9,
  ECMC_INTERLOCK_MAX_SPEED=10,
  ECMC_INTERLOCK_CONT_HIGH_LIMIT=11,
  ECMC_INTERLOCK_CONT_OUT_INCREASE_AT_LIMIT_SWITCH=12,
  ECMC_INTERLOCK_AXIS_ERROR_STATE=13,
  ECMC_INTERLOCK_UNEXPECTED_LIMIT_SWITCH_BEHAVIOUR=14,
  ECMC_INTERLOCK_VELOCITY_DIFF=15,
  ECMC_INTERLOCK_ETHERCAT_MASTER_NOT_OK=16,
};

enum encoderType{
  ECMC_ENCODER_TYPE_INCREMENTAL=0,
  ECMC_ENCODER_TYPE_ABSOLUTE=1,
};

enum commandType{
  ECMC_CMD_TYPE_EXECUTE=0,
  ECMC_CMD_TYPE_ENABLE=1,
};

enum transformVariableType{
  ECMC_TRANSFORM_VAR_TYPE_TRAJ=0,
  ECMC_TRANSFORM_VAR_TYPE_ENC=1,
  ECMC_TRANSFORM_VAR_TYPE_IL=2,
};

enum eventType{
  ECMC_SAMPLED = 0,
  ECMC_EDGE_TRIGGERED = 1,
};

enum triggerEdgeType{
  ECMC_POSITIVE_EDGE = 0,
  ECMC_NEGATIVE_EDGE = 1,
  ECMC_ON_CHANGE=2,
};

#endif /* ECMCDEFINITIONS_H_ */
