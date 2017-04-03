from distutils.core import setup, Extension

module = Extension('microservice',
                    sources=['c-micro/src/microservice.c', "routes.c"],
                    include_dirs=["c-micro/include"]
)

setup (name = 'MicroService',
       version = '1.0',
       description = 'This is the MicroService package',
       ext_modules = [module])
