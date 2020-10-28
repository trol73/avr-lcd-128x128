#include "gui.h"

#include "memory.h"

uint8_t gui_flags;

static void drawItemScreen();
static void drawItemList();

void guiDraw() {
	uint16_t offset = 0;
	while (true) {
		uint8_t itemType = gui_data[offset++];
		switch (itemType) {
			case GUI_ITEM_END:
				return;
			case GUI_ITEM_SCREEN:
				drawItemScreen();
				break;
			case GUI_ITEM_LIST:
				drawItemList();
				break;
		}
	}
}

bool guiKeyHandler(uint8_t key) {
	return false;
}


static void drawItemScreen() {

}

static void drawItemList() {

}
