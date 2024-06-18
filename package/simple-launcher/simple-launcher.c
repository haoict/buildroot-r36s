#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>


#define MAX_COMMAND_LENGTH 256
#define MAX_NAME_LENGTH 256


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Joystick *joystick;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char command[MAX_COMMAND_LENGTH];
} Command;

Command *commands = NULL;
int numCommands = 0;


void load_commands(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char line[MAX_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Allocate or reallocate commands array
        commands = realloc(commands, (numCommands + 1) * sizeof(*commands));
        if (commands == NULL) {
            printf("Could not allocate memory for commands\n");
            return;
        }

        // Copy name
        strncpy(commands[numCommands].name, line, MAX_NAME_LENGTH - 1);
        commands[numCommands].name[MAX_NAME_LENGTH - 1] = '\0';

        // Read next line for command
        if (fgets(line, sizeof(line), file) == NULL) {
            printf("Could not read command for %s\n", commands[numCommands].name);
            return;
        }

        // Remove trailing newline
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Copy command
        strncpy(commands[numCommands].command, line, MAX_COMMAND_LENGTH - 1);
        commands[numCommands].command[MAX_COMMAND_LENGTH - 1] = '\0';

        numCommands++;
    }

    fclose(file);
}

// Function to execute a shell script
void execute_shell_script(const char *script)
{
    // Close SDL window
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Execute shell script
    system(script);

    // Recreate SDL window
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    window = SDL_CreateWindow("Simple Launcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    joystick = SDL_JoystickOpen(0);
}


int main(int argc, char *argv[])
{
    TTF_Font *font;
    SDL_Color color = {255, 255, 255, 100};          // Low White color
    SDL_Color highlightColor = {255, 255, 255, 255}; // White color

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    TTF_Init();

    window = SDL_CreateWindow("DYI Launcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);
    font = TTF_OpenFont("./Ubuntu-R.ttf", 24); // Make sure you have this font file

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    joystick = SDL_JoystickOpen(0);

    load_commands("./simple-launcher-commands.txt");

    SDL_Event event;
    int running = 1;

    int selectedItem = 0;

    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 48, 48, 48, 255); // Set the color to gray
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                //printf("Key pressed: %d | %d\n", event.key.keysym.sym, event.key.keysym.scancode);
                // PC Keyboard
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    if (selectedItem > 0)
                        selectedItem--;
                    else
                        selectedItem = numCommands-1;
                    break;
                case SDLK_DOWN:
                    if (selectedItem < numCommands-1)
                        selectedItem++;
                    else
                        selectedItem = 0;
                    break;
                case SDLK_RETURN:
                    execute_shell_script(commands[selectedItem].command);
                    break;
                    // Add more keys as needed
                }
                // R36S Special keys
                switch (event.key.keysym.scancode)
                {
                case 128: // volume up
                case 129: // volume down
                case 102: // power
                    break;
                }
                break;
            case SDL_JOYAXISMOTION:
                if(event.jaxis.axis == 1) { // Y axis
                    if(event.jaxis.value < -10000) { // Up
                        if (selectedItem > 0)
                            selectedItem--;
                        else
                            selectedItem = numCommands-1;
                    } else if(event.jaxis.value > 10000) { // Down
                        if (selectedItem < numCommands-1)
                            selectedItem++;
                        else
                            selectedItem = 0;
                    }
                }
                break;
            case SDL_JOYBUTTONDOWN:
                //printf("Key pressed: %d\n", event.jbutton.button);
                // 1:A, 0:B, 2:X, 3:Y, 8: Up, 9: Down, 10: Left, 11: Right, 12: Select, 13: Start, 16: Fn, 14: L3, 15: R3, 4: L1, 5: R1, 6: L2, 7: R2   
                switch (event.jbutton.button) {
                case 1: // A
                    execute_shell_script(commands[selectedItem].command);
                    break;
                case 8: // Up
                    if (selectedItem > 0)
                        selectedItem--;
                    else
                        selectedItem = numCommands-1;
                    break;
                case 9: // Down
                    if (selectedItem < numCommands-1)
                        selectedItem++;
                    else
                        selectedItem = 0;
                    break;
                }
                break;
            }
        }

        // SDL_Surface *surface1 = TTF_RenderText_Solid(font, selectedItem == 0 ? "> Retroarch" : "Retroarch", selectedItem == 0 ? highlightColor : color);
        // SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
        // SDL_Rect rect4 = {20, 200, surface4->w, surface4->h};
        // SDL_RenderCopy(renderer, texture4, NULL, &rect4);
        SDL_Surface *surface[numCommands];
        SDL_Texture *texture[numCommands];
        for (int i = 0; i < numCommands; i++) {
            char str1[MAX_NAME_LENGTH] = "> ";
            surface[i] = TTF_RenderText_Solid(font, selectedItem == i ? strcat(str1, commands[i].name) : commands[i].name, selectedItem == i ? highlightColor : color);
            texture[i] = SDL_CreateTextureFromSurface(renderer, surface[i]);
            SDL_Rect rect = {20, 80 + i * 40, surface[i]->w, surface[i]->h};
            SDL_RenderCopy(renderer, texture[i], NULL, &rect);
        }


        // title
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "My CFW", color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        SDL_Texture* titleText = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
        SDL_Rect titleText_rect; //create a rect
        titleText_rect.x = 20;  //controls the rect's x coordinate 
        titleText_rect.y = 5; // controls the rect's y coordinte
        titleText_rect.w = 300; // controls the width of the rect
        titleText_rect.h = 50; // controls the height of the rect
        SDL_RenderCopy(renderer, titleText, NULL, &titleText_rect);


        SDL_RenderPresent(renderer);

        for (int i = 0; i < numCommands; i++) {
            SDL_FreeSurface(surface[i]);
            SDL_DestroyTexture(texture[i]);
        }
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(titleText);
    }

    free(commands);

    TTF_CloseFont(font);
    SDL_JoystickClose(joystick);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}