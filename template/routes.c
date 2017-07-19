#include "microservice.h"


// Return the original request, wrapped in our object.
json_response ping(json request) {
  return ok(kv("Original Request", request));
}


// Return a friendly "Hello world" response.
json_response hello() {
  return msg("Hello, world!");
}


// Route both methods to the "/" endpoint.
void routes(method get, method post) {
  route(post, "/", ping);
  route(get, "/", hello);
}

