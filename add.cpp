#include<iostream>
#include "contains.cpp"
#include "next_bucket.cpp"
using namespace std;

int* add(int *key,int *data){
  unsigned int hash=key&1023;
  Bucket* start_bucket=segments_ary[hash];
  start_bucket->lock();
  if(contains(key)){
    int *rc=data;
    start_bucket->unlock();
    return rc;
  }

  Bucket* free_bucket=start_bucket;
  int free_distance=0;
  for(;free_distance<ADD_RANGE;++free_distance){
    if(NULL==free_bucket->_key && NULL == ATOMIC_CAS(&(free_bucket->_key),NULL,BUSY))
      break;
    ++ free_bucket;
  }

  if(free_distance < ADD_RANGE){
    do{
      if(free_distance < HOP_RANGE){
        start_bucket->_hopInfo |= (1<<free_distance);
        free_bucket->_data=data;
        free_bucket->_key=key;
        start_bucket->unlock();
        return NULL;
      }
      find_closer_bucket(&free_bucket,&free_distance);
    }while(NULL != free_bucket);
  }
  start_bucket->unlock();
  return NULL;
}

