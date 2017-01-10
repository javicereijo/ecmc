/*
 * cMcuAxisVirt.h
 *
 *  Created on: Mar 14, 2016
 *      Author: anderssandstrom
 */

#ifndef ECMCAXISVIRT_H_
#define ECMCAXISVIRT_H_

#include "ecmcAxisBase.h"

//VIRTUAL AXIS
#define ERROR_VIRT_AXIS_TRAJ_NULL 0x14F00
#define ERROR_VIRT_AXIS_CREATE_TRANSFORM_INDEX_OUT_OF_RANGE 0x14F01
#define ERROR_VIRT_AXIS_INDEX_OUT_OF_RANGE 0x14F02
#define ERROR_VIRT_AXIS_LINKED_AXIS_TRAJ_NULL 0x14F03
#define ERROR_VIRT_AXIS_INDEX_NULL 0x14F04

class ecmcAxisVirt: public ecmcAxisBase
{
public:
  ecmcAxisVirt(int axisID, double sampleTime);
  ~ecmcAxisVirt();
  void execute(bool masterOK);
  int setOpMode(operationMode mode);
  operationMode getOpMode();
  int getCntrlError(double* error);
  int setExecute(bool execute);
  bool getExecute();
  int setEnable(bool enable);
  bool getEnable();
  void printStatus();
  ecmcDriveBase *getDrv();
  ecmcPIDController *getCntrl();
  int validate();

private:
  void initVars();
  bool initDone_;
};

#endif /* ECMCAXISVIRT_H_ */
