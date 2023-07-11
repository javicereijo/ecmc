/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcAxisSequencer.h
*
*  Created on: Jan 14, 2016
*      Author: anderssandstrom
*
\*************************************************************************/

#ifndef ecmcAxisSequencer_H_
#define ecmcAxisSequencer_H_

#include "../main/ecmcError.h"
#include "ecmcEncoder.h"
#include "ecmcMonitor.h"
#include "ecmcPIDController.h"
#include "ecmcTrajectoryBase.h"
#include "ecmcAxisData.h"
#include "ecmcDriveBase.h"

// SEQUENCER ERRORS
#define ERROR_SEQ_TRAJ_NULL 0x14D00
#define ERROR_SEQ_ENC_NULL 0x14D01
#define ERROR_SEQ_MON_NULL 0x14D02
#define ERROR_SEQ_CNTRL_NULL 0x14D03
#define ERROR_SEQ_SEQ_FAILED 0x14D04
#define ERROR_SEQ_COMMAND_NOT_SUPPORTED 0x14D05
#define ERROR_SEQ_SOFT_LIMIT_FWD 0x14D06
#define ERROR_SEQ_SOFT_LIMIT_BWD 0x14D07
#define ERROR_SEQ_TIMEOUT 0x14D08
#define ERROR_SEQ_CMD_UNDEFINED 0x14D09
#define ERROR_SEQ_CMD_DATA_UNDEFINED 0x14D0A
#define ERROR_SEQ_EXTERNAL_DATA_INTERFACE_NULL 0x14D0B
#define ERROR_SEQ_NO_HOME_SWITCH_FLANK 0x14D0C
#define ERROR_SEQ_NO_SECOND_HOME_SWITCH_FLANK 0x14D0D
#define ERROR_SEQ_ERROR_ABS_BIT_OUT_OF_RANGE 0x14D0E
#define ERROR_SEQ_ERROR_POSITION_SANITY_CHECK_FAILED 0x14D0F
#define ERROR_SEQ_ERROR_ACCELERATION_ZERO 0x14D10
#define ERROR_SEQ_ERROR_DECELERATION_ZERO 0x14D11
#define ERROR_SEQ_ERROR_VELOCITY_ZERO 0x14D12
#define ERROR_SEQ_ABS_OVER_UNDER_FLOW_ERROR 0x14D13
#define ERROR_SEQ_LATCH_COUNT_OUT_OF_RANGE 0x14D14
#define ERROR_SEQ_TARGET_POS_OUT_OF_RANGE 0x14D15
#define ERROR_SEQ_MOTION_CMD_NOT_ENABLED 0x14D16
#define ERROR_SEQ_HOME_POST_MOVE_FAILED 0x14D17
#define ERROR_SEQ_HOME_ENC_SOURCE_NOT_INTERNAL 0x14D18

// SEQUENCER WARNINGS
#define WARNING_SEQ_SETPOINT_SOFTLIM_FWD_VILOATION 0x114D00
#define WARNING_SEQ_SETPOINT_SOFTLIM_BWD_VILOATION 0x114D01
// Homing
enum ecmcHomingType {
  ECMC_SEQ_HOME_NOT_VALID                = 0,
  ECMC_SEQ_HOME_LOW_LIM                  = 1,
  ECMC_SEQ_HOME_HIGH_LIM                 = 2,
  ECMC_SEQ_HOME_LOW_LIM_HOME             = 3,
  ECMC_SEQ_HOME_HIGH_LIM_HOME            = 4,
  ECMC_SEQ_HOME_LOW_LIM_HOME_HOME        = 5,
  ECMC_SEQ_HOME_HIGH_LIM_HOME_HOME       = 6,
  ECMC_SEQ_HOME_BWD_HOME                 = 7,
  ECMC_SEQ_HOME_FWD_HOME                 = 8,
  ECMC_SEQ_HOME_BWD_HOME_HOME            = 9,
  ECMC_SEQ_HOME_FWD_HOME_HOME            = 10,
  ECMC_SEQ_HOME_LOW_LIM_INDEX            = 11,
  ECMC_SEQ_HOME_HIGH_LIM_INDEX           = 12,
  ECMC_SEQ_HOME_SET_POS                  = 15,
  ECMC_SEQ_HOME_BWD_NC_HOME              = 17,
  ECMC_SEQ_HOME_LOW_LIM_SINGLE_TURN_ABS  = 21,
  ECMC_SEQ_HOME_HIGH_LIM_SINGLE_TURN_ABS = 22,
  ECMC_SEQ_HOME_SET_POS_2                = 25,   // Same as ECMC_SEQ_HOME_SET_POS but not blocked by motor. Code handled in ecmcMotorRecordAxis
};

class ecmcAxisSequencer : public ecmcError {
 public:
  ecmcAxisSequencer();
  ~ecmcAxisSequencer();
  int                    setExecute(bool execute);
  bool                   getExecute();
  void                   execute();
  void                   setCommand(motionCommandTypes command);
  motionCommandTypes     getCommand();
  void                   setCmdData(int cmdData);
  int                    getCmdData();
  void                   setTraj(ecmcTrajectoryBase *traj);
  ecmcTrajectoryBase*    getTraj();
  void                   setEnc(ecmcEncoder **encArray);
  void                   setMon(ecmcMonitor *mon);
  void                   setDrv(ecmcDriveBase *drv);
  void                   setCntrl(ecmcPIDController *con);
  bool                   getBusy();
  void                   setJogVel(double velTarget);
  double                 getJogVel();
  int                    setHomeVelTowardsCam(double vel);
  int                    setHomeVelOffCam(double vel);
  double                 getHomeVelTowardsCam();
  double                 getHomeVelOffCam();
  void                   setHomePosition(double pos);
  double                 getHomePosition();

  // Home on hardware latch (index or external)
  // Homing will be made after <count> latches have been identified
  // only valid for certain home sequences
  void   setHomeLatchCountOffset(int count);
  void   setTargetPos(double pos);
  void   setTargetPos(double pos,
                      bool   force);
  double getTargetPos();
  void   setTargetVel(double velTarget);
  double getTargetVel();
  void   setJogFwd(bool jog);
  bool   getJogFwd();
  void   setJogBwd(bool jog);
  bool   getJogBwd();
  int    getSeqState();
  int    validate();
  int    setSequenceTimeout(int timeout);
  int    setExternalExecute(bool execute);
  int    setAxisDataRef(ecmcAxisData *data);
  int    setAllowMotionFunctions(bool enablePos,
                                 bool enableConstVel,
                                 bool enableHome);
  void   setHomePostMoveTargetPosition(double targetPos);
  void   setHomePostMoveEnable(double enable);
  void   setNewPositionCtrlDrvTrajBumpless(double newPosition);
  int    getAllowPos();
  int    getAllowConstVelo();
  int    getAllowHome();

 private:
  void   initVars();
  double checkSoftLimits(double posSetpoint);
  int    seqHoming1();   // nCmdData==1
  int    seqHoming2();   // nCmdData==2
  int    seqHoming3();   // nCmdData==3
  int    seqHoming4();   // nCmdData==4
  int    seqHoming5();   // nCmdData==5
  int    seqHoming6();   // nCmdData==6
  int    seqHoming7();   // nCmdData==7
  int    seqHoming8();   // nCmdData==8
  int    seqHoming9();   // nCmdData==9
  int    seqHoming10();  // nCmdData==10
  int    seqHoming11();  // nCmdData==11
  int    seqHoming12();  // nCmdData==12
  int    seqHoming15();  // nCmdData==15
  int    seqHoming17();  // nCmdData==17
  int    seqHoming21();  // nCmdData==21
  int    seqHoming22();  // nCmdData==22
  int    checkHWLimitsAndStop(bool checkBWD,
                              bool checkFWD);
  int    stopSeq();
  int    getExtTrajSetpoint(double *pos);
  int    checkVelAccDec();
  void   initHomingSeq();
  void   finalizeHomingSeq(double newPosition);
  int    postHomeMove();
  void   switchEncodersIfNeeded();
  void   switchBackEncodersIfNeeded();

  int seqState_;
  int seqStateOld_;
  int seqTimeout_;
  int seqTimeCounter_;
  int seqPosHomeState_;
  bool hwLimitSwitchFwd_;
  bool hwLimitSwitchFwdOld_;
  bool hwLimitSwitchBwd_;
  bool hwLimitSwitchBwdOld_;
  bool homeSensor_;
  bool homeSensorOld_;
  bool seqInProgress_;
  bool seqInProgressOld_;
  bool jogFwd_;
  bool jogBwd_;
  bool executeOld_;
  bool localSeqBusy_;
  bool homeEnablePostMove_;
  double homePostMoveTargetPos_;
  double jogVel_;
  double homeVelTowardsCam_;
  double homeVelOffCam_;
  double homePosition_;
  double homePosLatch1_;
  double homePosLatch2_;
  motionDirection currSeqDirection_;
  ecmcTrajectoryBase *traj_;
  ecmcEncoder **encArray_;
  ecmcMonitor *mon_;
  ecmcPIDController *cntrl_;
  ecmcDriveBase *drv_;
  //ecmcMasterSlaveIF *externalInputTrajectoryIF_;
  ecmcAxisData *data_;
  uint64_t oldencRawAbsPosReg_;
  uint64_t encRawAbsPosReg_;
  ecmcOverUnderFlowType overUnderFlowLatch_;
  int homeLatchCountOffset_;
  int homeLatchCountAct_;
  bool enablePos_;
  bool enableConstVel_;
  bool enableHome_;
  int oldPrimaryEnc_;
};

#endif  /* ecmcAxisSequencer_H_ */
