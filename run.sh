rm app
g++ main.cpp  -o app -lglut -lGLU -lGL objects.cpp utility.cpp config.cpp raytracer.cpp
./app