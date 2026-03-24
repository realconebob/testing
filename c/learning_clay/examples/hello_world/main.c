#define CLAY_IMPLEMENTATION
#include "clay.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

// Font ID constant
#define FONT_ID_HELLO 0

// Simple SDL2 text measurement function for Clay
Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData) {
    TTF_Font **fonts = (TTF_Font **)userData;
    TTF_Font *font = fonts[config->fontId];
    
    if (!font) {
        return (Clay_Dimensions) { 0, 0 };
    }
    
    // Create null-terminated string from Clay's string slice
    char *tempString = malloc(text.length + 1);
    memcpy(tempString, text.chars, text.length);
    tempString[text.length] = '\0';
    
    int width, height;
    TTF_SizeText(font, tempString, &width, &height);
    free(tempString);
    
    return (Clay_Dimensions) { (float)width, (float)height };
}

// Simple SDL2 renderer for Clay commands
void RenderClayCommands(SDL_Renderer *renderer, Clay_RenderCommandArray commands, TTF_Font **fonts) {
    for (int i = 0; i < commands.length; i++) {
        Clay_RenderCommand *cmd = &commands.internalArray[i];
        
        switch (cmd->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData *rect = &cmd->renderData.rectangle;
                SDL_SetRenderDrawColor(renderer, 
                    (Uint8)rect->backgroundColor.r,
                    (Uint8)rect->backgroundColor.g,
                    (Uint8)rect->backgroundColor.b,
                    (Uint8)rect->backgroundColor.a);
                SDL_Rect sdlRect = {
                    (int)cmd->boundingBox.x,
                    (int)cmd->boundingBox.y,
                    (int)cmd->boundingBox.width,
                    (int)cmd->boundingBox.height
                };
                SDL_RenderFillRect(renderer, &sdlRect);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextRenderData *text = &cmd->renderData.text;
                TTF_Font *font = fonts[text->fontId];
                
                // Create null-terminated string
                char *tempString = malloc(text->stringContents.length + 1);
                memcpy(tempString, text->stringContents.chars, text->stringContents.length);
                tempString[text->stringContents.length] = '\0';
                
                SDL_Color color = {
                    (Uint8)text->textColor.r,
                    (Uint8)text->textColor.g,
                    (Uint8)text->textColor.b,
                    (Uint8)text->textColor.a
                };
                
                SDL_Surface *surface = TTF_RenderText_Blended(font, tempString, color);
                if (surface) {
                    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect dstRect = {
                        (int)cmd->boundingBox.x,
                        (int)cmd->boundingBox.y,
                        surface->w,
                        surface->h
                    };
                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
                
                free(tempString);
                break;
            }
        }
    }
}

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("Clay Error: %.*s\n", (int)errorData.errorText.length, errorData.errorText.chars);
}

int main(int argc, char *argv[]) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }
    
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    
    // Create window
    SDL_Window *window = SDL_CreateWindow(
        "Clay + SDL2 - Hello World",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // Load font
    TTF_Font *fonts[1];
    fonts[FONT_ID_HELLO] = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 48);
    if (!fonts[FONT_ID_HELLO]) {
        fprintf(stderr, "Font loading error: %s\n", TTF_GetError());
        fprintf(stderr, "Note: Update font path for your system\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // Initialize Clay
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(clayMemory, (Clay_Dimensions) { 800, 600 }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(MeasureText, fonts);
    
    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // Update Clay with current window size
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        Clay_SetLayoutDimensions((Clay_Dimensions) { (float)windowWidth, (float)windowHeight });
        
        // Begin Clay layout
        Clay_BeginLayout();
        
        // Create a centered container with "Hello World!" text
        CLAY(CLAY_ID("OuterContainer"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = { 30, 30, 40, 255 }
        }) {
            CLAY(CLAY_ID("TextContainer"), {
                .layout = { .padding = { 32, 32, 32, 32 } },
                .backgroundColor = { 60, 60, 80, 255 },
                .cornerRadius = { 16, 16, 16, 16 }
            }) {
                CLAY_TEXT(
                    CLAY_STRING("Hello World!"),
                    CLAY_TEXT_CONFIG({
                        .fontId = FONT_ID_HELLO,
                        .fontSize = 48,
                        .textColor = { 255, 255, 255, 255 }
                    })
                );
            }
        }
        
        // End layout and get render commands
        Clay_RenderCommandArray renderCommands = Clay_EndLayout();
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);
        
        // Render Clay commands
        RenderClayCommands(renderer, renderCommands, fonts);
        
        // Present
        SDL_RenderPresent(renderer);
        
        // Small delay to prevent high CPU usage
        SDL_Delay(16);
    }
    
    // Cleanup
    TTF_CloseFont(fonts[FONT_ID_HELLO]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
