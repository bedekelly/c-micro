#include "Python.h"
#include "microservice.h"


json_response msg(char *s) {
  int len = strlen(s);
  char message[len + 20];
  sprintf(message, "{\"message\": \"%s\"}", s);
  char *heapMessage = strdup(message);
  int code = 200;
  json_response response = {code, heapMessage};
  return response;
}


json_response bad(json s) {
  int len = strlen(s);
  char message[len + 20];
  sprintf(message, "{\"error\": \"%s\"}", s);
  char *heapMessage = strdup(message);
  int code = 400;
  json_response response = {code, heapMessage};
  return response;
}

/**
 * Create a new "GET" endpoint on the heap.
 */
endpoint *makeGet(path routePath, getFunc handler) {
  endpoint *get = malloc(sizeof(endpoint));
  get->next = NULL;
  get->type = GET;
  get->location = strdup(routePath);
  get->getFunction = handler;
  return get;
}


/**
 * Create a new "POST" endpoint on the heap.
 */
endpoint *makePost(path routePath, postFunc handler) {
  endpoint *post = malloc(sizeof(endpoint));
  post->next = NULL;
  post->type = POST;
  post->location = strdup(routePath);
  post->postFunction = handler;
  return post;
}


/**
 * This function is part of the API, and can be called by
 * clients in their `route()` function.
 */
void route(method m, path routePath, routeFunc handler) {
  endpoint *ep = *m;
  
  switch(ep->type) {
  case GET:
    {
      while (ep->next) ep = ep->next;
      ep->next = makeGet(routePath, handler);
      break;
    }
  case POST:
    {
      while (ep->next) ep = ep->next;
      ep->next = makePost(routePath, handler);
      break;
    }
  }
}


/**
 * Create a dummy GET endpoint for the start of the linked list.
 */
endpoint *emptyGet() {
  endpoint *get = malloc(sizeof(endpoint));
  get->next = NULL;
  get->type = GET;
  get->location = NULL;
  return get;
}


/**
 * Create a dummy POST endpoint for the start of the linked list.
 */
endpoint *emptyPost() {
  endpoint *post = malloc(sizeof(endpoint));
  post->next = NULL;
  post->type = POST;
  post->location = NULL;
  return post;
}


/**
 * Wrapper method to allow Flask to delegate to our C GET endpoints.
 */
endpoint *getList, *postList;
static PyObject *
microservice_get(PyObject *self, PyObject *args) {
  char *input;
  if (!PyArg_ParseTuple(args, "s", &input)) {
    printf("Couldn't parse input.\n");
    return NULL;
  }
  endpoint *ep = getList->next;
  while (ep) {
    if (!strcmp(ep->location, input)) {
      json_response resp = ep->getFunction();
      PyObject *response = Py_BuildValue("(is)", resp.code, resp.text);
      return response;
    }
    ep = ep->next;
  }
  return NULL;
}


/**
 * Wrapper method to allow Flask to delegate to our C POST endpoints.
 */
static PyObject *
microservice_post(PyObject *self, PyObject *args) {
  json data;
  path routePath;
  if (!PyArg_ParseTuple(args, "ss", &routePath, &data)) {
    printf("Couldn't parse input.\n");
    return NULL;
  }
  endpoint *ep = postList->next;

  while (ep) {
    if (!strcmp(ep->location, routePath)) {
      json_response resp = ep->postFunction(data);
      PyObject *response = Py_BuildValue("(is)", resp.code, resp.text);
      return response;
    }
    ep = ep->next;
  }
  return NULL;
}


/**
 * Initialize the module so we can access its attribute through Flask.
 */
PyMODINIT_FUNC PyInit_microservice(void)
{
  // Define all the methods we expose in this module.
  static PyMethodDef microserviceMethods[] = {
    {"get", microservice_get, METH_VARARGS, "call a GET endpoint"},
    {"post", microservice_post, METH_VARARGS, "call a POST endpoint"},
    {NULL, NULL, 0, NULL}
  };

  // Define some metadata about the module.
  static struct PyModuleDef microservice_module = {
    PyModuleDef_HEAD_INIT,
    "microservice",
    NULL,
    -1,  // Important: we keep state in global variables!
    microserviceMethods
  };

  // Create the module and error-check.
  PyObject *m;
  m = PyModule_Create(&microservice_module);
  if (m == NULL) return NULL;
  

  // Load all user-defined routes.
  getList = emptyGet(), postList = emptyPost();
  routes(&getList, &postList);

  // Create a Set object for each method: PySet_New();
  PyObject *getSet;  // go!
  getSet = PySet_New(NULL);
  PyObject *postSet = PySet_New(NULL);

  // For each route, create a String object: Py_BuildValue("s", s);
  endpoint *ep = getList->next;
  while (ep) {
    PyObject *getPath = Py_BuildValue("s", ep->location);

    // Add that String object to the relevant method Set. PySet_Add
    PySet_Add(getSet, getPath);
    ep = ep->next;
  }

  // Repeat this process with the list of POST endpoints.
  ep = postList->next;
  while (ep) {
    PyObject *postPath = Py_BuildValue("s", ep->location);
    PySet_Add(postSet, postPath);
    ep = ep->next;
  }

  // Add the resources we've just built to our module's attributes.
  PyModule_AddObject(m, "get_resources", getSet);
  PyModule_AddObject(m, "post_resources", postSet);

  // Finally, return our newly created module!
  return m;
}


int main(int argc, char* argv[]) {
  wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    /* Add a built-in module, before Py_Initialize */
    PyImport_AppendInittab("microservice", PyInit_microservice);

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Optionally import the module; alternatively,
       import can be deferred until the embedded script
       imports it. */
    // PyImport_ImportModule("microservice");

    return 0;
}
