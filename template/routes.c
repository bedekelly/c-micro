#include "microservice.h"


json_response ping(json request) {
  return ok(request);
}

json_response hello() {
  return msg("Hello, world!");
}

void routes(method get, method post)
{
  route(post, "/", ping);
  route(get, "/", hello);
}

