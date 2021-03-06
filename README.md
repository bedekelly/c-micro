# C-Micro

A simple, flexible framework for writing microservices in C.

...*No, really.*

### How?

All the hard stuff is in Python/Flask. But the API provided is in C, with plenty of utility functions like `route(method, path, function)`.

In the spirit of Flask, here's the simplest possible "Hello World" application:

```c
#include "microservice.h"

json_response hello() {
  return msg("Hello, world!");
}

void routes(method get, method post)
{
  route(get, "/", hello);
}
```

### Why?!

Because I like to suffer. Also, it'd be cool to benchmark this against a pure-Flask microservice doing something intensive like fibonnacci. #webscale

## Getting Started

```bash
# Download and install the wrapper.
mkdir myproject && cd myproject
git clone https://github.com/bedekelly/c-micro
cp c-micro/template/* .
make install

# Compile and run with the latest changes!
make run
```

A simple echo-server should be running on `localhost:8080`! Test it out using `curl`, `httpie` or Postman.

![](screenshot.png)

## Details

Each project needs a `routes.c` file. This `routes.c` file must contain a function with this signature:

```c
void routes(method get, method post);
````

Inside this function, calls to the `route` function should be made in order to register handler functions for endpoints.

```C
void routes(method get, method post) {
    route(get, "/", helloWorld);
    route(post, "todo", addTodo);
}
```

The signature for a handler function should look like the following:

```C
json_response getResource();
json_response addResource(json data);
```

To return a `json_response`, utility functions are provided in `microservice.h`. These include:

```C

// Returns a JSON response with code 200.
json_response ok(json message);

// Returns a JSON response with provided code.
json_response err(json message, int errorCode);

// Returns a JSON response with provided JSON.
json_response msg(char *message);

// Returns a single key-value JSON object.
json kv(char *key, char *value);
```
