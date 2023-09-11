#include <SDL.h>
#include <cmath>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const double PI = 3.14159265358979323846;
const double g = 9.81;  // Gravity

// Pendulum properties
double m1 = 10.0, m2 = 10.0;  // Masses
double l1 = 150.0, l2 = 150.0;  // Lengths of the rods
double theta1 = PI / 2, theta2 = PI / 2;  // Initial angles
double p1 = 0.0, p2 = 0.0;  // Momentum
double dt = 0.01;  // Time step

// Function to update the angles and momenta using the equations of motion
void update() {
    double sin_diff = sin(theta1 - theta2);
    double cos_diff = cos(theta1 - theta2);

    double denom1 = l1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2));
    double denom2 = l2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2));

    // Calculate the accelerations (d²θ/dt²)
    double theta1_acc = (-g * (2 * m1 + m2) * sin(theta1)
                        - m2 * g * sin(theta1 - 2 * theta2)
                        - 2 * sin_diff * m2 * (p2 * p2 * l2 + p1 * p1 * l1 * cos_diff))
                        / denom1;

    double theta2_acc = (2 * sin_diff * (p1 * p1 * l1 * (m1 + m2) 
                        + g * (m1 + m2) * cos(theta1) + p2 * p2 * l2 * m2 * cos_diff)) 
                        / denom2;

    // Update momenta based on the accelerations
    p1 += theta1_acc * dt;
    p2 += theta2_acc * dt;

    // Update angles based on momenta
    theta1 += p1 * dt;
    theta2 += p2 * dt;
}

// Function to convert from polar to Cartesian coordinates
void polarToCartesian(double theta1, double theta2, double& x1, double& y1, double& x2, double& y2) {
    x1 = SCREEN_WIDTH / 2 + l1 * sin(theta1);
    y1 = SCREEN_HEIGHT / 2 + l1 * cos(theta1);
    x2 = x1 + l2 * sin(theta2);
    y2 = y1 + l2 * cos(theta2);
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Double Pendulum Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update the pendulum's motion
        update();

        // Convert polar coordinates to Cartesian for rendering
        double x1, y1, x2, y2;
        polarToCartesian(theta1, theta2, x1, y1, x2, y2);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw first pendulum rod
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, x1, y1);

        // Draw second pendulum rod
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

        // Draw the first bob
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect bob1 = {(int)x1 - 10, (int)y1 - 10, 20, 20};
        SDL_RenderFillRect(renderer, &bob1);

        // Draw the second bob
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect bob2 = {(int)x2 - 10, (int)y2 - 10, 20, 20};
        SDL_RenderFillRect(renderer, &bob2);

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Control frame rate
        SDL_Delay(5);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
