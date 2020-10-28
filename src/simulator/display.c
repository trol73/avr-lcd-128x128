#include <unistd.h>

#include "display.h"
//#include "../debug.h"
#include "savepng.h"
#include "../config.h"
#include "../lcd.h"
//#include "../general.h"
#include "server.h"
#include "util/delay.h"
#include "../keyboard.h"


#define KEY_MASK_UP		1
#define KEY_MASK_DOWN	2
#define KEY_MASK_LEFT	4
#define KEY_MASK_RIGHT	8
#define KEY_MASK_ENTER	16

#define ZOOM			4



// rrrrrggg:gggbbbbb

static uint16_t lcd_buffer[LCD_WIDTH * LCD_HEIGHT];
static uint8_t simulator_area_x;
static uint8_t simulator_area_y;
static uint8_t simulator_area_w;
static uint8_t simulator_area_h;
static uint8_t simulator_x;
static uint8_t simulator_y;

static uint16_t rgbColor(int r, int g, int b) {
	r = (0x1f * r / 0xff) & 0x1f;
	g = (0x3f * g / 0xff) & 0x3f;
	b = (0x1f * b / 0xff) & 0x1f;

	int result = b & 0x1f;
	result += ((g & 0x3f) << 5);
	result += (r & 0x1f) << 11;

	return result;
}

static int colorR(int color) {
	return ((color >> 11) & 0b00011111)*0xff/0b00011111;
}

static int colorG(int color) {
	return ((color >> 5) & 0b00111111)*0xff/0b00111111;
}

static int colorB(int color) {
	return ((color) & 0b00011111)*0xff/0b00011111;
}


uint8_t key_pressed_mask = 0;
uint8_t key_mask = 0;

SDL_Window * window;
SDL_Renderer *ren;
volatile Uint32 lastPressedTime = 0;

//SDL_TimerID timer2comp;
SDL_TimerID timer1ms;

//PaStream *audio_stream;


char simulator_cwd[1024];
uint16_t display_highlight_val;

static bool screenshoot_request = false;
static uint8_t zoomRequest = 0;

static uint8_t zoom = ZOOM;

void SimulatorInit();
void SimulatorDone();



void render_sdl() {
	SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 255);

	SDL_RenderClear(ren);

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	SDL_Rect r;
	r.w = zoom;
	r.h = zoom;

	bool highlight = display_highlight_val == 0xffff;
	if (display_highlight_val && lastPressedTime) {
		highlight = true;
	}
	uint8_t alpha = 0xff;
	for (uint8_t x = 0; x < LCD_WIDTH; x++) {
		for (uint8_t y = 0; y < LCD_HEIGHT; y++) {
			int color = lcd_buffer[y*LCD_WIDTH + x];
			SDL_SetRenderDrawColor(ren, colorR(color), colorG(color), colorB(color), alpha);
			//SDL_RenderDrawPoint(ren, x, y);
			r.x = x*zoom + 4;
			r.y = y*zoom + 4;
			SDL_RenderFillRect(ren, &r);
		}
	}
	SDL_RenderPresent(ren);

	if (screenshoot_request) {
		screenshoot_request = false;
		SDL_Surface *sshot = SDL_CreateRGBSurface(0, getWindowWidth(), getWindowHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		SDL_RenderReadPixels(ren, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
		char filename[256];
		snprintf(filename, sizeof(filename), "screenshot-%i.png", SDL_GetTicks());
		SDL_SavePNG(sshot, filename);
		SDL_FreeSurface(sshot);
	}
	if (zoomRequest) {
		switch (zoomRequest) {
			case 1:
				zoom = 1;
				break;
			case 2:
				zoom = 2;
				break;
			case 3:
				zoom = 4;
				break;
		}
		SDL_SetWindowSize(window, getWindowWidth(), getWindowHeight());
		zoomRequest = 0;
	}
	usleep(10);
}

void simulator_draw_pixel(uint8_t x, uint8_t y, uint16_t color) {
	if (x >= LCD_WIDTH || y >= LCD_HEIGHT) {
		printf("Pixel out of area: simulator_draw_pixel(%i, %i)\n", x, y);
	} else {
		lcd_buffer[y*LCD_WIDTH + x] = color;
	}
}

void simulator_draw_data_start(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	if (x >= LCD_WIDTH || y >= LCD_HEIGHT) {
		printf("Wrong area start point: (%i, %i)\n", x, y);
	} else if (w == 0 || h == 0 || w > LCD_WIDTH || h > LCD_HEIGHT) {
		printf("Wrong area start size: (%i, %i)\n", w, h);
	} else {
		simulator_area_x = x;
		simulator_area_y = y;
		simulator_area_w = w;
		simulator_area_h = h;
		simulator_x = x;
		simulator_y = y;
	}
}

void simulator_draw_next_pixel(uint16_t color) {
	simulator_draw_pixel(simulator_x, simulator_y, color);
	simulator_x++;
	if (simulator_x >= simulator_area_x + simulator_area_w) {
		simulator_x = simulator_area_x;
		simulator_y++;
	}
}


uint8_t getKeyMask(SDL_Event event) {
	switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			return KEY_MASK_LEFT;
		case SDLK_RIGHT:
			return KEY_MASK_RIGHT;
		case SDLK_UP:
			return KEY_MASK_UP;
		case SDLK_DOWN:
			return KEY_MASK_DOWN;
		case SDLK_RETURN:
		case SDLK_RETURN2:
			return KEY_MASK_ENTER;
	}
	return 0;
}


//uint8_t simulator_pool_event() {
////	usleep(1);
//	SDL_Event event;
//	if (!SDL_PollEvent(&event)) {
//		return 0;
//	}
//	uint8_t mask;
//	switch (event.type) {
//		case SDL_QUIT:
//			SimulatorDone();
//			exit(0);
//			break;
//		case SDL_KEYDOWN:
//			//printf( "Key press detected\n" );
//			key_pressed_mask |= getKeyMask(event);
//			lastPressedTime = SDL_GetTicks();
//			if (event.key.keysym.sym == SDLK_F1) {
//				screenshoot_request = true;
//			}
//			return key_pressed_mask;
//		case SDL_KEYUP:
//			//printf( "Key release detected\n" );
//			mask = getKeyMask(event);
//			key_pressed_mask &= ~mask;
//			lastPressedTime = SDL_GetTicks();
//			return key_pressed_mask;
//	}
//	return 0;
//}

static void updateKeyboardRegisters() {
	if (key_pressed_mask & KEY_MASK_UP) {
		PINC &= ~_BV(4);
	} else {
		PINC |= _BV(4);
	}

	if (key_pressed_mask & KEY_MASK_DOWN) {
		PINC &= ~_BV(2);
	} else {
		PINC |= _BV(2);
	}

	if (key_pressed_mask & KEY_MASK_LEFT) {
		PINC &= ~_BV(1);
	} else {
		PINC |= _BV(1);
	}

	if (key_pressed_mask & KEY_MASK_RIGHT) {
		PINC &= ~_BV(5);
	} else {
		PINC |= _BV(5);
	}

	if (key_pressed_mask & KEY_MASK_ENTER) {
		PINC &= ~_BV(3);
	} else {
		PINC |= _BV(3);
	}
	for (int i = 0; i < 5; i++) {
		KeyboardCheck();
	}
}

bool simulator_pool_event() {
//	usleep(1);
	SDL_Event event;
	bool result = false;
	while (SDL_PollEvent(&event)) {
		uint8_t mask;
		switch (event.type) {
			case SDL_QUIT:
				printf("Quit\n");
				SimulatorDone();
				exit(0);
				break;
			case SDL_KEYDOWN:
				//printf( "Key press detected\n" );
				key_pressed_mask |= getKeyMask(event);
				updateKeyboardRegisters();
				lastPressedTime = SDL_GetTicks();
				if (event.key.keysym.sym == SDLK_F1) {
					screenshoot_request = true;
				} else if (event.key.keysym.sym >= SDLK_F2 && event.key.keysym.sym <= SDLK_F4) {
					zoomRequest = event.key.keysym.sym - SDLK_F2 + 1;
				}

				result = true;
				break;

				//return key_pressed_mask;
			case SDL_KEYUP:
				//printf( "Key release detected\n" );
				mask = getKeyMask(event);
				key_pressed_mask &= ~mask;
				updateKeyboardRegisters();
				lastPressedTime = SDL_GetTicks();
				result = true;
				break;
		}
	}
	return result;
}



//Uint32 timer2comp_callback(Uint32 interval, void *param) {
//	bool hl = display_simulator_highlight;
//	display_check_hardware();
//	if (display_simulator_highlight != hl) {
//		glcd_write();
//	}
//	TIMER2_COMP_vect();
//	return interval;
//}

Uint32 timer1ms_callback(Uint32 interval, void *param) {
	if (lastPressedTime) {
		if (SDL_GetTicks() - lastPressedTime > display_highlight_val*5) {
			lastPressedTime = 0;
//			render_sdl();
		}
	}
	usleep(100);
	return interval;
}

//static int paCallback(const void *inputBuffer, void *outputBuffer,
//		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
//		PaStreamCallbackFlags statusFlags, void *userData) {
//
//	float *out = (float*)outputBuffer;
//	float v = OCR3BL ? 1.0 : 0;
//	*out++ = v;
//	*out++ = v;
//
//	return paContinue;
//}

int getWindowWidth() {
	return LCD_WIDTH * zoom + 8;
}

int getWindowHeight() {
	return LCD_HEIGHT * zoom + 8;
}

void SimulatorInit() {
	getcwd(simulator_cwd, sizeof(simulator_cwd));
	fprintf(stdout, "Current working dir: %s\n", simulator_cwd);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
		return;
	}

	window = SDL_CreateWindow("Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			  getWindowWidth(), getWindowHeight(), SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "\nSDL_CreateWindow Error:  %s\n", SDL_GetError());
		return;
	}

	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderClear(ren);

//	timer2comp = SDL_AddTimer(10, timer2comp_callback, NULL);
//
	timer1ms = SDL_AddTimer(1, timer1ms_callback, NULL);
//
//	PaError err = Pa_Initialize();
//	if (err != paNoError) {
//		printf("Error: can't initialize PortAudio");
//	}
//
//
//	// ----------------------------------------------------------------------
//	PaStreamParameters outputParameters;
//
//	outputParameters.device = Pa_GetDefaultOutputDevice(); // default output device
//	if (outputParameters.device == paNoDevice) {
//      printf("Error: No default output device.\n");
//		return;
//	}
//	outputParameters.channelCount = 2;       // stereo output
//	outputParameters.sampleFormat = paFloat32; // 32 bit floating point output
//	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
//	outputParameters.hostApiSpecificStreamInfo = NULL;
//
//	err = Pa_OpenStream(
//              &audio_stream,
//              NULL, // no input
//              &outputParameters,
//              44100,
//              2,	// FRAMES_PER_BUFFER,
//              paClipOff,      // we won't output out of range samples so don't bother clipping them
//              paCallback,
//              NULL);
//
//	if (err != paNoError) {
//		printf("Error: can't open stream\n");
//		return;
//	}
//	err = Pa_StartStream(audio_stream);
//	if (err != paNoError) {
//		 printf("Error: can't start stream\n");
//		 return;
//	}
	render_sdl();
	server_start();


	printf("Simulator started\n");
}


void SimulatorDone() {
//	PaError err = Pa_StopStream(audio_stream);
//	if (err != paNoError) {
//		 printf("Error: can't stop stream\n");
//	}
//
//	err = Pa_CloseStream(audio_stream);
//	if (err != paNoError) {
//		printf("Error: can't close stream\n");
//	}
//	Pa_Terminate();
	server_stop();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



int simulator_get_next_byte() {
	if (simulator_pool_event()) {
		render_sdl();
		_delay_ms(1);
	}

	int cmd = server_read_next_byte();
	if (cmd >= 0) {
		return cmd;
	}
	_delay_ms(1);
	return -1;
}


void simulator_to_front() {
	SDL_RaiseWindow(window);
}