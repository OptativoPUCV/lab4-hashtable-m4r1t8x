#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  if(map == NULL || key == NULL || value == NULL) return;
  long posicion = hash(key,map->capacity);
  if(map->buckets[posicion] == NULL)
  {
    map->buckets[posicion] = createPair(key,value);
    map->size++;
    map->current = posicion;
  }  
  else
  {
    while(map->buckets[posicion] != NULL)
      {
        posicion++;
        if(posicion == map->capacity) posicion = 0;
      }
    map->buckets[posicion] = createPair(key,value);
    map->size++;
    map->current = posicion;
  }
  


}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair ** aux = map->buckets;
  map->capacity *= 2;
  map->buckets = (Pair **)malloc(sizeof(Pair *) * map->capacity);
  map->size = 0;
  for(int i = 0; i < map->capacity/2; i++)
    {
      if(aux[i] != NULL)
      {
        insertMap(map, aux[i]->key, aux[i]->value); 
      }
    }
  for(int i = 0; i < map->capacity/2; i++)
    {
      if(aux[i] != NULL)
      {
        free(aux[i]);
      }
    }
  free(aux);
}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity,sizeof(Pair *));
  map->capacity = capacity;
  map->size = 0;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) 
{
  if(map == NULL || key == NULL ) return;
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion] != NULL)
    {
      if(is_equal(map->buckets[posicion]->key,key))
      {
        map->buckets[posicion]->key = NULL;
        map->size--;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
    }
}

Pair * searchMap(HashMap * map,  char * key) 
{
  if(map == NULL || key == NULL) return NULL;
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
    {
      if(is_equal(map->buckets[posicion]->key,key))
      {
        map->current = posicion;
        return map->buckets[posicion];
      }
      posicion = (posicion + 1) % map->capacity;
    }
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
  if(map == NULL) return NULL;
  if(map->buckets[0] != NULL && map->buckets[0]->key != NULL)
  {
    map->current = 0;
    return map->buckets[0];
  }
  else
  {
    for(int i = 1; i < map->capacity; i++)
      {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
          map->current = i;
          return map->buckets[i];
        }
      }
  }
  return NULL;
}

Pair * nextMap(HashMap * map)
{
  if(map == NULL) return NULL;
  if(map->buckets[map->current + 1] != NULL && map->buckets[map->current + 1]->key != NULL)
  {
    map->current++;
    return map->buckets[map->current];
  }
  else
  {
    for(int i = map->current + 2; i < map->capacity; i++)
      {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
          map->current = i;
          return map->buckets[i];
        }
      }
  }
  return NULL;
}
