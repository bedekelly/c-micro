#include "microservice.h"


json_response ping() {
  return msg("Hello, world!");
}


json_response error() {
  return bad("Error Message!");
}


void routes(method get, method post)
{
  route(get, "/ping", ping);
  route(get, "/error", error);
}

