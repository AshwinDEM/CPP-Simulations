# Simulations

This is a repository to store my basic simulations that I want to improve on. \
This also serves as a template for my future sdl projects. 

### Use
The src folder has many cpp files, all with a main function. They are individual simulations.\
To run one, adjust the makefile to use that file:
```
all:
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o Main src/<current_file>.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
```
Replace <current_file> with the cpp file that you want to use. For example, to use the mandelbrot file:
```
all:
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o Main src/mandelbrot.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
```

After that, run `make`, and execute the `Main.exe` file.