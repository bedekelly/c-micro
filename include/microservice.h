#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef char* json;
typedef char* path;

typedef struct json_response {
  int code;
  json text;
} json_response;

json_response ok(json msg);

typedef json_response getFunc();
typedef json_response postFunc(json);

typedef void* routeFunc;

enum endpointType { GET, POST };

typedef struct endpoint {
  enum endpointType type;
  struct endpoint *next;
  path location;
  union {
    getFunc *getFunction;
    postFunc *postFunction;
  };
} endpoint;

typedef endpoint** method;

void routes(method, method);
endpoint *emptyGet();

endpoint *emptyPost();


// These methods comprise the API:
json_response ok(json message);
json_response err(json message, int errorCode);
json_response msg(char *message);
json kv(char *key, char *value);
json ks(char *key, char *value);
void route(method m, path location, routeFunc handler);
