#include "gfx.h"


// WidgetStyle: Header
const GWidgetStyle color_header = {
	HTML2COLOR(0x0000b3),              // background
	HTML2COLOR(0x2A8FCD),              // focus

	// Enabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x404040),         // edge
		HTML2COLOR(0x0000b3),         // fill
		HTML2COLOR(0x00E000),         // progress (inactive area)
	},

	// Disabled color set
	{
		HTML2COLOR(0xC0C0C0),         // text
		HTML2COLOR(0x808080),         // edge
		HTML2COLOR(0xE0E0E0),         // fill
		HTML2COLOR(0xC0E0C0),         // progress (active area)
	},

	// Pressed color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x404040),         // edge
		HTML2COLOR(0x808080),         // fill
		HTML2COLOR(0x00E000),         // progress (active area)
	}
};


// WidgetStyle: Body (black)
const GWidgetStyle color_black = {
	HTML2COLOR(0x000000),              // background
	HTML2COLOR(0x000000),              // focus

	// Enabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x000000),         // edge
		HTML2COLOR(0x000000),         // fill
		HTML2COLOR(0x000000),         // progress (inactive area)
	},

	// Disabled color set
	{
		HTML2COLOR(0xC0C0C0),         // text
		HTML2COLOR(0x000000),         // edge
		HTML2COLOR(0x000000),         // fill
		HTML2COLOR(0x000000),         // progress (active area)
	},

	// Pressed color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x000000),         // edge
		HTML2COLOR(0x000000),         // fill
		HTML2COLOR(0x000000),         // progress (active area)
	}
};


// WidgetStyle: Buttons
const GWidgetStyle color_buttons = {
	HTML2COLOR(0xFFFFFF),              // background
	HTML2COLOR(0x0000b3),              // focus

	// Enabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x000080),         // edge
		HTML2COLOR(0x0000b3),         // fill
		HTML2COLOR(0x00FFFF),         // progress (inactive area)
	},

	// Disabled color set
	{
		HTML2COLOR(0x003300),         // text
		HTML2COLOR(0x000000),         // edge
		HTML2COLOR(0x000000),         // fill
		HTML2COLOR(0x000000),         // progress (active area)
	},

	// Pressed color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x000080),         // edge
		HTML2COLOR(0x0000FF),         // fill
		HTML2COLOR(0xFFFFFF),         // progress (active area)
	}
};

// WidgetStyle: White
const GWidgetStyle color_one = {
	HTML2COLOR(0xFFFFFF),              // background
	HTML2COLOR(0x2A8FCD),              // focus

	// Enabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x404040),         // edge
		HTML2COLOR(0x3088BF),         // fill
		HTML2COLOR(0x00E000),         // progress (inactive area)
	},

	// Disabled color set
	{
		HTML2COLOR(0xC0C0C0),         // text
		HTML2COLOR(0x808080),         // edge
		HTML2COLOR(0xE0E0E0),         // fill
		HTML2COLOR(0xC0E0C0),         // progress (active area)
	},

	// Pressed color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x404040),         // edge
		HTML2COLOR(0x808080),         // fill
		HTML2COLOR(0x00E000),         // progress (active area)
	}
};