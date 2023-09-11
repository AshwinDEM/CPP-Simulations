#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>

const double sigma = 10.0;
const double rho = 28.0;
const double beta = 8.0 / 3.0;
const double dt = 0.01;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

void lorenz(double& x, double& y, double& z) {
    double dx = sigma * (y - x);
    double dy = x * (rho - z) - y;
    double dz = x * y - beta * z;
    
    x += dx * dt;
    y += dy * dt;
    z += dz * dt;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Lorenz Attractor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    double x1 = 0.01, y1 = 0.0, z1 = 0.0;
    double x2 = 0.02, y2 = 0.0, z2 = 0.0;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        if (quit) break;

        for (int i = 0; i < 100; ++i) {
            lorenz(x1, y1, z1);
            lorenz(x2, y2, z2);

            int pointX1 = (int)((x1 + 25) * (SCREEN_WIDTH / 50));
            int pointY1 = (int)((z1 + 0) * (SCREEN_HEIGHT / 50));
            int pointX2 = (int)((x2 + 25) * (SCREEN_WIDTH / 50));
            int pointY2 = (int)((z2 + 0) * (SCREEN_HEIGHT / 50));

            if (pointX1 >= 0 && pointX1 < SCREEN_WIDTH && pointY1 >= 0 && pointY1 < SCREEN_HEIGHT) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
                SDL_RenderDrawPoint(renderer, pointX1, pointY1);
            }

            if (pointX2 >= 0 && pointX2 < SCREEN_WIDTH && pointY2 >= 0 && pointY2 < SCREEN_HEIGHT) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                SDL_RenderDrawPoint(renderer, pointX2, pointY2);
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }

        // SDL_RenderPresent(renderer);
        // SDL_Delay(500);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
