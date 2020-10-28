#ifndef _VGI_CODES_H_
#define _VGI_CODES_H_

#define VGI_END				0	// (void)
#define VGI_SET_COLOR		1	// (color: u16)
#define VGI_DRAW_LINE		2	// (x: u8, y: u8)
#define VGI_DRAW_POLYLINE	3	// (n:u8, [x: u8, y:u8]*n)
#define VGI_SET_FONT			4	// (font: u8)
#define VGI_DRAW_TEXT		5	// (x: u8, y: u8, [cstr])
#define VGI_DRAW_RECT		6	// (x: u8, y: u8, w: u8, h: u8)
#define VGI_FILL_RECT		7	// (x: u8, y: u8, w: u8, h: u8)
#define VGI_DRAW_CIRCLE		8	// (x: u8, y: u8, r: u8)
#define VGI_FILL_CIRCLE		9	// (x: u8, y: u8, r: u8)
#define VGI_SET_BACKGROUND 10	// (color: u16)
#define VGI_DRAW_MONO_8X8	11 // (data: [u8]*8)
#define VGI_FILL_MONO_8X8	12 // (data: [u8]*8)
#define VGI_DRAW_GRID		13 // (x: u8, y: u8, dx: u8, dy: u8, nx: u8, ny: u8)
#define VGI_DRAW_UINT8		14	// (x: u8, y: u8, val: u8)

#endif // _VGI_CODES_H_
