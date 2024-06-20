#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_NAME_LENGTH 256

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Joystick *joystick;

TTF_Font *xsFont;
TTF_Font *sFont;
TTF_Font *mFont;
TTF_Font *lFont;
TTF_Font *xlFont;
TTF_Font *titleFont;

typedef struct {
	char name[MAX_NAME_LENGTH];
	char command[MAX_COMMAND_LENGTH];
} Command;

Command *commands = NULL;
int numCommands = 0;
char title[MAX_NAME_LENGTH];
char batteryCapacity[8];
char batteryCapacityDisplayString[20] = "Batt: 0%";
char brightness[8];
char brightnessDisplayString[20] = "Brightness: 0%";
char volume[8];
char volumeDisplayString[20] = "Volume: 0%";
char creditDisplayString[MAX_NAME_LENGTH] = "TiniLinux";

void load_commands(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		printf("Could not open file %s\n", filename);
		return;
	}

	char line[MAX_COMMAND_LENGTH];

	if (fgets(title, sizeof(title), file) == NULL) {
		printf("Could not read title\n");
		return;
	}
	title[strlen(title) - 1] = '\0';

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

void updateHwInfo()
{
	// Update battery percentage
	FILE *batteryFile = fopen("/sys/class/power_supply/battery/capacity", "r");
	if (batteryFile == NULL) {
		printf("Could not open battery file\n");
	} else {
		fgets(batteryCapacity, sizeof(batteryCapacity), batteryFile);
		fclose(batteryFile);
		batteryCapacity[strlen(batteryCapacity) - 1] = '\0';
		strcpy(batteryCapacityDisplayString, "Batt: ");
		strcat(batteryCapacityDisplayString, batteryCapacity);
		strcat(batteryCapacityDisplayString, "%");
	}
	// Read brightness
	FILE *brightnessFile = fopen("/sys/devices/platform/backlight/backlight/backlight/brightness", "r");
	if (brightnessFile == NULL) {
		printf("Could not open brightness file\n");
	} else {
		fgets(brightness, sizeof(brightness), brightnessFile);
		fclose(brightnessFile);
		strcpy(brightnessDisplayString, "Brightness: ");
		brightness[strlen(brightness) - 1] = '\0';
		strcat(brightnessDisplayString, brightness);
	}
	// Read volume
	FILE *pipe = popen("amixer get Playback | awk -F'[][]' '/Left:/ { print $2 }'", "r");
	if (pipe == NULL) {
		printf("Failed to open pipe\n");
	} else {
		fgets(volume, sizeof(volume), pipe);
		pclose(pipe);
		if (volume == NULL || strlen(volume) == 0) {
			strcpy(volume, "0%\n");
		}
		volume[strlen(volume) - 1] = '\0';
		strcpy(volumeDisplayString, "Volume: ");
		strcat(volumeDisplayString, volume);
	}
	strcpy(creditDisplayString, "TiniLinux (R36S) | ");
	strcat(creditDisplayString, brightnessDisplayString);
	strcat(creditDisplayString, " | ");
	strcat(creditDisplayString, volumeDisplayString);
}

void updateRender(int selectedItem, SDL_Color color, SDL_Color highlightColor)
{
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255); // Set the color to gray
	SDL_RenderClear(renderer);

	// menu items
	SDL_Surface *surface[numCommands];
	SDL_Texture *texture[numCommands];
	for (int i = 0; i < numCommands; i++) {
		char str1[MAX_NAME_LENGTH] = "> ";
		surface[i] = TTF_RenderText_Blended(mFont, selectedItem == i ? strcat(str1, commands[i].name) : commands[i].name,
						    selectedItem == i ? highlightColor : color);
		texture[i] = SDL_CreateTextureFromSurface(renderer, surface[i]);
		SDL_Rect rect = { 20, 80 + i * 40, surface[i]->w, surface[i]->h };
		SDL_RenderCopy(renderer, texture[i], NULL, &rect);
	}

	// title
	SDL_Surface *titleSurface = TTF_RenderText_Blended(titleFont, title, color);
	SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
	SDL_Rect titleText_rect = { 20, 10, titleSurface->w, titleSurface->h };
	SDL_RenderCopy(renderer, titleTexture, NULL, &titleText_rect);

	// battery
	SDL_Surface *batterySurface = TTF_RenderText_Blended(sFont, batteryCapacityDisplayString, color);
	SDL_Texture *batteryTexture = SDL_CreateTextureFromSurface(renderer, batterySurface);
	SDL_Rect batteryRect = { 530, 5, batterySurface->w, batterySurface->h };
	SDL_RenderCopy(renderer, batteryTexture, NULL, &batteryRect);

	// Credit
	SDL_Surface *creditSurface = TTF_RenderText_Blended(xsFont, creditDisplayString, color);
	SDL_Texture *creditTexture = SDL_CreateTextureFromSurface(renderer, creditSurface);
	SDL_Rect creditRect = { 20, 445, creditSurface->w, creditSurface->h };
	SDL_RenderCopy(renderer, creditTexture, NULL, &creditRect);

	// render all
	SDL_RenderPresent(renderer);

	// free all
	for (int i = 0; i < numCommands; i++) {
		SDL_FreeSurface(surface[i]);
		SDL_DestroyTexture(texture[i]);
	}
	SDL_FreeSurface(titleSurface);
	SDL_DestroyTexture(titleTexture);
	SDL_FreeSurface(batterySurface);
	SDL_DestroyTexture(batteryTexture);
	SDL_FreeSurface(creditSurface);
	SDL_DestroyTexture(creditTexture);
}

// Function to execute a shell script
void execute_shell_script(const char *script)
{
	// Close SDL window
	SDL_DestroyWindow(window);
	SDL_Quit();

	// Execute shell script
	system("printf \"\\033c\" > /dev/tty1");
	system(script);

	updateHwInfo();

	// Recreate SDL window
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	window = SDL_CreateWindow("Simple Launcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	joystick = SDL_JoystickOpen(0);
}

int main(int argc, char *argv[])
{
	SDL_Color color = { 255, 255, 255, 80 }; // Low White color
	SDL_Color highlightColor = { 255, 255, 255, 255 }; // White color

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	TTF_Init();

	window = SDL_CreateWindow("DYI Launcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	xsFont = TTF_OpenFont("./simple-launcher.ttf", 16);
	sFont = TTF_OpenFont("./simple-launcher.ttf", 20);
	mFont = TTF_OpenFont("./simple-launcher.ttf", 24);
	lFont = TTF_OpenFont("./simple-launcher.ttf", 28);
	xlFont = TTF_OpenFont("./simple-launcher.ttf", 32);
	titleFont = TTF_OpenFont("./simple-launcher.ttf", 44);
	if (xsFont == NULL || sFont == NULL || mFont == NULL || lFont == NULL || xlFont == NULL || titleFont == NULL) {
		printf("Failed to load font: %s\n", TTF_GetError());
		return 1;
	}

	joystick = SDL_JoystickOpen(0);

	load_commands("./simple-launcher-commands.txt");

	SDL_Event event;
	int running = 1;
	int selectedItem = 0;

	updateHwInfo();

	updateRender(selectedItem, color, highlightColor);

	// main loop
	while (running) {
		while (SDL_PollEvent(&event)) {
			printf("Event type: %d\n", event.type);

			switch (event.type) {
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				// printf("Key pressed: %d | %d\n", event.key.keysym.sym, event.key.keysym.scancode);
				// PC Keyboard
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					if (selectedItem > 0)
						selectedItem--;
					else
						selectedItem = numCommands - 1;
					break;
				case SDLK_DOWN:
					if (selectedItem < numCommands - 1)
						selectedItem++;
					else
						selectedItem = 0;
					break;
				case SDLK_RETURN:
					execute_shell_script(commands[selectedItem].command);
					break;
				}
				// R36S Special keys
				switch (event.key.keysym.scancode) {
				case 128: // volume up
				case 129: // volume down
					SDL_Delay(200);
					updateHwInfo();
					break;
				case 102: // power
					break;
				}
				break;
			case SDL_JOYAXISMOTION:
				if (event.jaxis.axis == 1) { // Y axis
					if (event.jaxis.value < -10000) { // Up
						if (selectedItem > 0)
							selectedItem--;
						else
							selectedItem = numCommands - 1;
					} else if (event.jaxis.value > 10000) { // Down
						if (selectedItem < numCommands - 1)
							selectedItem++;
						else
							selectedItem = 0;
					}
				}
				break;
			case SDL_JOYBUTTONDOWN:
				SDL_JoystickUpdate();
				if (SDL_JoystickGetButton(joystick, 16)) { // Fn button is being pressed down
					SDL_Delay(200);
					updateHwInfo();
					break;
				}
				// printf("Key pressed: %d\n", event.jbutton.button);
				// 1:A, 0:B, 2:X, 3:Y, 8: Up, 9: Down, 10: Left, 11: Right, 12: Select, 13: Start, 16: Fn, 14: L3, 15: R3, 4: L1, 5: R1, 6: L2, 7: R2
				switch (event.jbutton.button) {
				case 1: // A
					execute_shell_script(commands[selectedItem].command);
					break;
				case 0: // B
					selectedItem = 0;
					break;
				case 8: // Up
					if (selectedItem > 0)
						selectedItem--;
					else
						selectedItem = numCommands - 1;
					break;
				case 9: // Down
					if (selectedItem < numCommands - 1)
						selectedItem++;
					else
						selectedItem = 0;
					break;
				}

				break;
			}
		}

		updateRender(selectedItem, color, highlightColor);
	}

	free(commands);

	TTF_CloseFont(xsFont);
	TTF_CloseFont(sFont);
	TTF_CloseFont(mFont);
	TTF_CloseFont(lFont);
	TTF_CloseFont(xlFont);
	TTF_CloseFont(titleFont);
	SDL_JoystickClose(joystick);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
