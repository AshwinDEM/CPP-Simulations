#include <SDL.h>
#include <complex>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MAX_ITER = 1000;

// Mandelbrot function
int mandelbrot(std::complex<double> c) {
    std::complex<double> z = 0;
    int iter = 0;
    while (std::abs(z) <= 2 && iter < MAX_ITER) {
        z = z * z + c;
        iter++;
    }
    return iter;
}

// Map pixel (x, y) to the complex plane
std::complex<double> pixelToComplex(int x, int y) {
    double real = (double)x / SCREEN_WIDTH * (3.5) - 2.5;
    double imag = (double)y / SCREEN_HEIGHT * (2.0) - 1.0;
    return std::complex<double>(real, imag);
}

// Function to map iterations to an RGB color
void getColor(int iter, int& r, int& g, int& b) {
    if (iter == MAX_ITER) {
        // Inside Mandelbrot set: Color it black
        r = 0;
        g = 0;
        b = 0;
    } else {
        // Outside Mandelbrot set: Use a color gradient
        double t = (double)iter / MAX_ITER;
        r = (int)(9 * (1 - t) * t * t * t * 255);   // Red component
        g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);  // Green component
        b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255); // Blue component
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Mandelbrot Set with Colors",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Generate Mandelbrot fractal
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            std::complex<double> c = pixelToComplex(x, y);
            int iter = mandelbrot(c);

            // Get color based on the number of iterations
            int r, g, b;
            getColor(iter, r, g, b);

            // Set the pixel color
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    // Update the screen
    SDL_RenderPresent(renderer);

    // Wait for 5 seconds
    SDL_Delay(5000);

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
