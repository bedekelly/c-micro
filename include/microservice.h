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



json_response msg(json s);
json_response bad(json s);


void routes(method, method);

void route(method m, path location, routeFunc handler);

endpoint *emptyGet();

endpoint *emptyPost();
