/*
 * ecmcDataStorage.cpp
 *
 *  Created on: May 27, 2016
 *      Author: anderssandstrom
 */

#include "ecmcDataStorage.h"

ecmcDataStorage::ecmcDataStorage (int index)
{
  initVars();
  buffer_=new double[ECMC_DEFAULT_DATA_STORAGE_SIZE];
  index_=index;
  if(buffer_==NULL){
    LOGINFO9("%s/%s:%d: ERROR: Data storage %d. Buffer allocation failed (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,index_,ERROR_DATA_STORAGE_NULL);
    setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_DATA_STORAGE_NULL);
  }
}

ecmcDataStorage::ecmcDataStorage (int index, int size,storageType bufferType)
{
  initVars();
  bufferElementCount_=size;
  buffer_=new double[size];
  index_=index;
  if(buffer_==NULL){
    LOGINFO9("%s/%s:%d: ERROR: Data storage %d. Buffer allocation failed (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,index_,ERROR_DATA_STORAGE_NULL);
    setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_DATA_STORAGE_NULL);
  }
  bufferType_=bufferType;
}

ecmcDataStorage::~ecmcDataStorage ()
{
  delete buffer_;
}

void ecmcDataStorage::initVars()
{
  bufferType_=ECMC_STORAGE_LIFO_BUFFER;
  bufferElementCount_=ECMC_DEFAULT_DATA_STORAGE_SIZE;
  buffer_=NULL;
  currentBufferIndex_=0;
  index_=0;
}

int ecmcDataStorage::clearBuffer()
{
  if(buffer_==NULL){
    LOGINFO9("%s/%s:%d: ERROR: Data storage %d. Clear buffer failed. Buffer NULL (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,index_,ERROR_DATA_STORAGE_NULL);
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_DATA_STORAGE_NULL);
  }
  memset(buffer_,0,bufferElementCount_*sizeof(double));
  currentBufferIndex_=0;
  return 0;
}

int ecmcDataStorage::setBufferSize(int elements)
{
  delete buffer_;
  bufferElementCount_=elements;
  buffer_=new double[bufferElementCount_];
  return 0;
}

int ecmcDataStorage::isStorgeFull()
{
  return currentBufferIndex_==bufferElementCount_;
}

int ecmcDataStorage::getSize()
{
  return bufferElementCount_;
}

int ecmcDataStorage::printBuffer()
{
  int start=0;
  int end=bufferElementCount_;
  if(bufferType_==ECMC_STORAGE_LIFO_BUFFER){
    end=currentBufferIndex_;
  }
  printf("Printout of data storage buffer %d.\n",index_);
  for(int i=start;i<end ;i++){
    printf("%lf, ",buffer_[i]);
  }
  printf("\n");
  return 0;
}

int ecmcDataStorage::getData(double **data, int *size)
{
  *data=buffer_;
  *size=bufferElementCount_;
  return 0;
}

int ecmcDataStorage::setData(double *data, int size)
{
  currentBufferIndex_=0; //Start from beginning
  return appendData(data,size);
}

int ecmcDataStorage::appendData(double *data, int size)
{
  if(buffer_==NULL){
    LOGINFO9("%s/%s:%d: ERROR: Data storage %d. Append data failed. Buffer NULL (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,index_,ERROR_DATA_STORAGE_NULL);
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_DATA_STORAGE_NULL);
  }

  if(size>bufferElementCount_){
    LOGINFO9("%s/%s:%d: ERROR: Data storage %d. Buffer size to small (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,index_,ERROR_DATA_STORAGE_SIZE_TO_SMALL);
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_DATA_STORAGE_SIZE_TO_SMALL);
  }

  int sizeToCopy=size;
  if(sizeToCopy>bufferElementCount_){
    sizeToCopy=bufferElementCount_;
  }
  if(sizeToCopy>bufferElementCount_-currentBufferIndex_){
    sizeToCopy=bufferElementCount_-currentBufferIndex_;
  }

  if(sizeToCopy>0){

    for(int i=0;i<sizeToCopy;i++){
      LOGINFO9("%s/%s:%d: INFO: Data storage %d. Appending %lf at index %d.\n",__FILE__, __FUNCTION__, __LINE__,index_,*(data+i), currentBufferIndex_+i);
    }

    memcpy(buffer_+currentBufferIndex_,data ,sizeToCopy*sizeof(double));
    currentBufferIndex_=currentBufferIndex_+sizeToCopy;
  }

  if(sizeToCopy<size){
    if(bufferType_!=ECMC_STORAGE_RING_BUFFER){
      LOGINFO9("%s/%s:%d: Error: Data storage %d. Data storage full (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,index_,ERROR_DATA_STORAGE_FULL);
      return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_DATA_STORAGE_FULL);
    }

    for(int i=0;i<size-sizeToCopy;i++){
      LOGINFO9("%s/%s:%d: INFO: Data storage %d. Appending %lf at index %d.\n",__FILE__, __FUNCTION__, __LINE__,index_,*(data+sizeToCopy+i), i);
    }

    //If ring buffer then copy the rest of the data to the beginning of the buffer
    memcpy(buffer_, data+sizeToCopy ,(size-sizeToCopy)*sizeof(double));
    currentBufferIndex_=(size-sizeToCopy);
  }

  return 0;
}

int ecmcDataStorage::appendData(double data)
{
  return appendData(&data,1);
}

int ecmcDataStorage::setCurrentPosition(int position)
{
  if(position>bufferElementCount_ || position<0){
    return setErrorID(__FILE__,__FUNCTION__,__LINE__,ERROR_DATA_STORAGE_POSITION_OUT_OF_RANGE);
  }
  currentBufferIndex_=position;
  return 0;
}
