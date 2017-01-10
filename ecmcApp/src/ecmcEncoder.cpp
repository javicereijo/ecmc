/*
 * ecmcEncoder.cpp
 *
 *  Created on: Mar 14, 2016
 *      Author: anderssandstrom
 */

#include "ecmcEncoder.h"

ecmcEncoder::ecmcEncoder(double sampleTime) : ecmcEcEntryLink()
{
  initVars();
  sampleTime_=sampleTime;
  velocityFilter_=new ecmcFilter(sampleTime_);
}

ecmcEncoder::~ecmcEncoder()
{
  delete velocityFilter_;
}

void ecmcEncoder::initVars()
{
  errorReset();
  encType_=ECMC_ENCODER_TYPE_INCREMENTAL;
  rawPos_=0;
  range_=0;
  limit_=0;
  bits_=0;
  rawPosUintOld_=0;
  rawPosUint_=0;
  turns_=0;
  scale_=0;
  offset_=0;
  actPos_=0;
  actPosOld_=0;
  sampleTime_=1;
  actVel_=0;
  homed_=false;
  scaleNum_=0;
  scaleDenom_=1;
}

int64_t ecmcEncoder::getRawPos()
{
  return rawPos_;
}

int ecmcEncoder::setScaleNum(double scaleNum)
{
  scaleNum_=scaleNum;
  if(std::abs(scaleDenom_)>0){
    scale_=scaleNum_/scaleDenom_;
  }
  return 0;
}

int ecmcEncoder::setScaleDenom(double scaleDenom)
{
  scaleDenom_=scaleDenom;
  if(scaleDenom_==0){
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_SCALE_DENOM_ZERO);
  }
  scale_=scaleNum_/scaleDenom_;
  return 0;
}

double ecmcEncoder::getScale()
{
  return scale_;
}

double ecmcEncoder::getActPos()
{
  return actPos_;
}

void ecmcEncoder::setActPos(double pos)
{
  //Must clear velocity filter
  velocityFilter_->reset();
  actPosOld_=actPos_;
  offset_=offset_+pos-actPos_;
}

void ecmcEncoder::setSampleTime(double sampleTime)
{
  sampleTime_=sampleTime;
  velocityFilter_->setSampleTime(sampleTime_);
}

void ecmcEncoder::setOffset(double offset)
{
  offset_=offset;
}

double ecmcEncoder::getSampleTime()
{
  return sampleTime_;
}

double ecmcEncoder::getActVel()
{
  return actVel_;
}

void ecmcEncoder::setHomed(bool homed)
{
  homed_=homed;
}

bool ecmcEncoder::getHomed()
{
  return homed_;
}

int ecmcEncoder::setType(encoderType encType)
{
  switch(encType){
    case ECMC_ENCODER_TYPE_ABSOLUTE:
      encType_=encType;
      homed_=true;
      break;
    case ECMC_ENCODER_TYPE_INCREMENTAL:
      encType_=encType;
      break;
    default:
      return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_TYPE_NOT_SUPPORTED);
      break;
  }
  validate();
  return 0;
}

encoderType ecmcEncoder::getType()
{
  return encType_;
}

int64_t ecmcEncoder::handleOverUnderFlow(uint64_t newValue, int bits)
{
  rawPosUintOld_=rawPosUint_;
  rawPosUint_=newValue;
  if(bits_<64){//Only support for over/under flow of datatypes less than 64 bit
    if(rawPosUintOld_>rawPosUint_ && rawPosUintOld_-rawPosUint_>limit_){//Overflow
      turns_++;
    }
    else{
      if(rawPosUintOld_<rawPosUint_ &&rawPosUint_-rawPosUintOld_ >limit_){//Underflow
        turns_--;
      }
    }
  }
  else{
    turns_=0;
  }

  return turns_*range_+rawPosUint_;
}

int ecmcEncoder::setBits(int bits)
{
  bits_=bits;
  range_=pow(2,bits_);
  limit_=range_*2/3;  //Limit for change in value
  return 0;
}

double ecmcEncoder::getScaleNum()
{
  return scaleNum_;
}

double ecmcEncoder::getScaleDenom()
{
  return scaleDenom_;
}

double ecmcEncoder::readEntries()
{
  actPosOld_=actPos_;

  if(getError()){
      rawPos_=0;
      actPos_=scale_*rawPos_+offset_;
      return actPos_;
  }


  int error=validateEntry(ECMC_ENCODER_ENTRY_INDEX_ACTUAL_POSITION);
  if(error){
    setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_ENTRY_NULL);
    return 0;
  }

  //Act position
  uint64_t tempRaw=0;

  if(readEcEntryValue(ECMC_ENCODER_ENTRY_INDEX_ACTUAL_POSITION,&tempRaw)){
    setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_ENTRY_READ_FAIL);
    return actPos_;
  }
  rawPos_=handleOverUnderFlow(tempRaw,bits_) ;
  actPosOld_=actPos_;
  actPos_=scale_*rawPos_+offset_;
  //actVel_=velocityFilter_->lowPassAveraging((actPos_-actPosOld_)/sampleTime_);
  actVel_=velocityFilter_->positionBasedVelAveraging(actPos_);

  return actPos_;
}

int ecmcEncoder::validate()
{

  if(sampleTime_<=0){
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_INVALID_SAMPLE_TIME);
  }

  if(scaleDenom_==0){
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_SCALE_DENOM_ZERO);
  }

  if(encType_!=ECMC_ENCODER_TYPE_ABSOLUTE && encType_!= ECMC_ENCODER_TYPE_INCREMENTAL){
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_TYPE_NOT_SUPPORTED);
  }

  int errorCode=validateEntry(ECMC_ENCODER_ENTRY_INDEX_ACTUAL_POSITION);
  if(errorCode){   //Act position
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_ENC_ENTRY_NULL);
  }


  return 0;
}
