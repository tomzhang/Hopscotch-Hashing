#include<iostream>
#include "hopscotch.cpp"
#include <pthread.h>

using namespace std;

Hopscotch *obj;

int key;
int val;

void *remove(void* x){
  key = 0;
  cout << "Calling remove with key : " << key << endl;
  int* data = obj->remove(&key);
  cout << "FINISHED remove, Result : " << *data << endl;
  obj->trial();
}

void *add(void* x){

  key = 0;
  val = 100;

  cout << "ADD Start" << key << endl;  
  obj->add(&key, &val);
  cout << "ADD done" << key << endl;  

  pthread_exit(NULL);
}


void *contains(void* x){

  int key = 0;
  cout << "contains : " << key << endl;  
  bool present = obj->contains(&key);
  cout << "contains? : " << present << endl;
}

void addContains(){
  cout << "CONCURRENT ADD AND CONTAINS" << endl;
  int rc;

  pthread_t threads[40];
  rc = pthread_create(&threads[0], NULL, add, (void *)(obj));
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
  }
  sleep(1);
  for(int t=1; t<=5; t++){
    rc = pthread_create(&threads[t], NULL, contains, (void *)(obj));
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
    }
  }

  for(int t=0; t<4; t++){
    pthread_join(threads[t], NULL);
  } 
}


void addRemove()
{
  cout << "CONCURRENT ADD AND REMOVE" << endl;
  int rc;
  pthread_t threads[2];
  rc = pthread_create(&threads[0], NULL, add, (void *)(0));
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
  }
  rc = pthread_create(&threads[1], NULL, remove, (void *)(1));
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
  }
  
  
  for(int t=0; t<2; t++){
    pthread_join(threads[t], NULL);
  }
}

int main(){
  obj = new Hopscotch;
  addRemove();
  addContains();
  return 0;
}
