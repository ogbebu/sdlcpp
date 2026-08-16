#include <SDL3/SDL.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "Entity.hpp"
#include <SDL3_image/SDL_image.h>

void draw_imgui();
void draw();
void calculate_delta_time();

struct AppState {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool vsync = false;
    double lastTime = 0.0;
    double deltaTime = 0.0;
    Uint64 freq = SDL_GetPerformanceFrequency();
};

struct GameState {
    Entity player1{};
    Entity player2{};
};

AppState appState{};
GameState gameState{};


int main() {
    // --- SDL init ---
    SDL_Init(SDL_INIT_VIDEO);
    appState.window = SDL_CreateWindow(
        "SDL yo", //title
        1280, //resx
        720, //resy
        SDL_WINDOW_RESIZABLE // window flags 
    );
    appState.renderer = SDL_CreateRenderer(appState.window, nullptr);
    // ------------------- 


    // --- ImGui init ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(appState.window, appState.renderer);
    ImGui_ImplSDLRenderer3_Init(appState.renderer);
    // -------------------


    // --- game loop / main loop ---
    bool running = true;
    appState.lastTime = SDL_GetPerformanceCounter();
    SDL_Event event; 

    gameState.player1.texture = IMG_LoadTexture(appState.renderer, "../textures/BebuRect.png");
    gameState.player2.texture = IMG_LoadTexture(appState.renderer, "../textures/RupamRect.png");

    // gameState.player2.size = vec2(500,500);

    gameState.player1.speed = 350.0f;
    gameState.player2.speed = 350.0f;
    // do ctrl + shift + p then CMak

    while (running) {
        gameState.player1.direction = vec2(0, 0);
        gameState.player2.direction = vec2(0, 0);

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event); 

            if (event.type == SDL_EVENT_QUIT) 
                running = false;

        }

        const bool* keystate = SDL_GetKeyboardState(nullptr);

        if (!io.WantCaptureKeyboard) {
            // Player 1 Controls
            if (keystate[SDL_SCANCODE_W]) gameState.player1.direction += vec2(0, -1);
            if (keystate[SDL_SCANCODE_S]) gameState.player1.direction += vec2(0, 1);
            if (keystate[SDL_SCANCODE_D]) gameState.player1.direction += vec2(1, 0);
            if (keystate[SDL_SCANCODE_A]) gameState.player1.direction += vec2(-1, 0);

            // Player 2 Controls
            if (keystate[SDL_SCANCODE_UP]) gameState.player2.direction += vec2(0, -1);
            if (keystate[SDL_SCANCODE_DOWN]) gameState.player2.direction += vec2(0, 1);
            if (keystate[SDL_SCANCODE_RIGHT]) gameState.player2.direction += vec2(1, 0);
            if (keystate[SDL_SCANCODE_LEFT]) gameState.player2.direction += vec2(-1, 0);
        }
        calculate_delta_time();
        draw(); // main draw functions
    }
    // -------------------


    // --- cleanup ---
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(appState.renderer);
    SDL_DestroyWindow(appState.window);
    SDL_Quit();

    return 0;
}

void calculate_delta_time() {
    Uint64 currentTime = SDL_GetPerformanceCounter();
    appState.deltaTime = (currentTime - appState.lastTime) / appState.freq; // seconds
    appState.lastTime = currentTime;
}

void draw_imgui() {
    // Start ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::Text("graphics api: %s", SDL_GetRendererName(appState.renderer));
    ImGui::Text("fps: %.3f", ImGui::GetIO().Framerate);

    // if (ImGui::Checkbox("v-sync", &appState.vsync)) {
    //     SDL_SetRenderVSync(appState.renderer, appState.vsync ? 1 : 0);
    // } 

    ImGui::Text("direction p1: x: %f, y: %f", gameState.player1.direction.x, gameState.player1.direction.y);
    ImGui::Text("direction p2: x: %f, y: %f", gameState.player2.direction.x, gameState.player2.direction.y);

    ImGui::Text("deltatime: %.6f", appState.deltaTime);

    ImGui::End();
    ImGui::Render();
}

// Main Drawing Function 
void draw() {
    draw_imgui();

    // Drawing Background
    SDL_SetRenderDrawColor(appState.renderer, 100, 100, 100, 255);
    SDL_RenderClear(appState.renderer);

    // updating players data
    gameState.player1.pos += gameState.player1.direction * gameState.player1.speed * appState.deltaTime;
    gameState.player2.pos += gameState.player2.direction * gameState.player2.speed * appState.deltaTime;

    bool reverse = fmod(appState.lastTime / appState.freq, 2.0) > 1.0;

    float direction = reverse ? -1.0f : 1.0f;

    gameState.player1.size.x += 50.0f * appState.deltaTime * -direction;
    gameState.player1.size.y += 50.0f * appState.deltaTime * direction;

    gameState.player2.size.x += 50.0f * appState.deltaTime * direction;
    gameState.player2.size.y += 50.0f * appState.deltaTime * -direction;

    // Drawing Player 1
    SDL_SetRenderDrawColor(appState.renderer, 255, 0, 0, 255);
    SDL_FRect rect1 = { gameState.player1.pos.x, gameState.player1.pos.y, gameState.player1.size.x, gameState.player1.size.y };
    SDL_RenderTexture(appState.renderer, gameState.player1.texture, nullptr, &rect1);

    // Drawing Player 2
    SDL_SetRenderDrawColor(appState.renderer, 0, 255, 0, 255);
    SDL_FRect rect2 = { gameState.player2.pos.x, gameState.player2.pos.y, gameState.player2.size.x, gameState.player2.size.y };
    SDL_RenderTexture(appState.renderer, gameState.player2.texture, nullptr, &rect2);

    // Drawing ImGui
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), appState.renderer);

    SDL_RenderPresent(appState.renderer);
}