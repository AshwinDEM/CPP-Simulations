#include <SDL.h>
#include <vector>
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 100;
const double alpha = 0.01;  // Thermal diffusivity constant
const double dt = 0.1;      // Time step
const double dx = 1.0;      // Space step

std::vector<std::vector<double>> temperature(GRID_SIZE, std::vector<double>(GRID_SIZE, 0.0));
std::vector<std::vector<double>> newTemperature(GRID_SIZE, std::vector<double>(GRID_SIZE, 0.0));

// Function to initialize the temperature grid with a central hot source
void initializeTemperature() {
    int centerX = GRID_SIZE / 2;
    int centerY = GRID_SIZE / 2;
    int radius = 10; // Define the size of the hot source (bigger red dot)

    // Set a square region around the center to a high temperature
    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            if (centerX + i >= 0 && centerX + i < GRID_SIZE && centerY + j >= 0 && centerY + j < GRID_SIZE) {
                temperature[centerX + i][centerY + j] = 1000.0;
            }
        }
    }
}

// Function to update the temperature grid using the heat diffusion equation
void updateTemperature() {
    for (int i = 1; i < GRID_SIZE - 1; ++i) {
        for (int j = 1; j < GRID_SIZE - 1; ++j) {
            newTemperature[i][j] = temperature[i][j] + alpha * dt * (
                (temperature[i + 1][j] + temperature[i - 1][j] + temperature[i][j + 1] + temperature[i][j - 1] - 4 * temperature[i][j]) / (dx * dx)
            );
        }
    }

    // Copy newTemperature into temperature
    temperature = newTemperature;
}

// Function to map a temperature value to a color (blue for cold, red for hot)
// Function to map a temperature value to a color (black for cold, green for hot)
void getColor(double temp, int& r, int& g, int& b) {
    double normalizedTemp = std::min(1.0, temp / 1000.0); // Normalize temperature between 0 and 1
    
    r = (int)(255 * normalizedTemp);                             // No red component
    g = 0;   // Green intensity increases with heat
    b = 0;                             // No blue component
    
    // Cold parts (near zero temperature) are black
}

// Function to create a heat source at the given grid coordinates (x, y)
void createHeatSource(int gridX, int gridY, int radius = 5) {
    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            int newX = gridX + i;
            int newY = gridY + j;
            if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
                temperature[newX][newY] = 1000.0; // Set temperature to a high value
            }
        }
    }
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Heat Diffusion Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    initializeTemperature(); // Set initial temperature

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Handle mouse click
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Convert mouse position to grid coordinates
                int gridX = mouseX * GRID_SIZE / SCREEN_WIDTH;
                int gridY = mouseY * GRID_SIZE / SCREEN_HEIGHT;

                // Create a new heat source at the clicked position
                createHeatSource(gridX, gridY);
            }
        }

        // Update the temperature grid
        updateTemperature();

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the heat map
        int cellWidth = SCREEN_WIDTH / GRID_SIZE;
        int cellHeight = SCREEN_HEIGHT / GRID_SIZE;

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                int r, g, b;
                getColor(temperature[i][j], r, g, b);
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_Rect rect = { i * cellWidth, j * cellHeight, cellWidth, cellHeight };
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        // Present the updated screen
        SDL_RenderPresent(renderer);

        // Slow down the simulation for visibility
        SDL_Delay(30);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
