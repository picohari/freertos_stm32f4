#include "gfx.h"


// WidgetStyle: White
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


// WidgetStyle: White
const GWidgetStyle color_white = {
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


// WidgetStyle: White
const GWidgetStyle color_green = {
	HTML2COLOR(0x000000),              // background
	HTML2COLOR(0x2A8FCD),              // focus

	// Enabled color set
	{
		HTML2COLOR(0x00CC00),         // text
		HTML2COLOR(0x404040),         // edge
		HTML2COLOR(0x000000),         // fill
		HTML2COLOR(0x000000),         // progress (inactive area)
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
		HTML2COLOR(0x000000),         // edge
		HTML2COLOR(0x000000),         // fill
		HTML2COLOR(0x000000),         // progress (active area)
	}
};






const GWidgetStyle axislabels_unhomed = {
	HTML2COLOR(0x000000),              // background
	HTML2COLOR(0xFFFFFF),              // focus

	// Enabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x00CC00),         // edge
		HTML2COLOR(0x00CC00),         // fill
		HTML2COLOR(0x00CC00),         // progress (inactive area)
	},

	// Disabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x0000b3),         // edge
		HTML2COLOR(0x0000b3),         // fill
		HTML2COLOR(0x0000b3),         // progress (active area)
	},

	// Pressed color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0xCCCC00),         // edge
		HTML2COLOR(0xCCCC00),         // fill
		HTML2COLOR(0xCCCC00),         // progress (active area)
	}
};

const GWidgetStyle axislabels_homed = {
	HTML2COLOR(0x000000),              // background
	HTML2COLOR(0xFFFFFF),              // focus

	// Enabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x00006E),         // edge
		HTML2COLOR(0x00006E),         // fill
		HTML2COLOR(0x00006E),         // progress (inactive area)
	},

	// Disabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x0000b3),         // edge
		HTML2COLOR(0x0000b3),         // fill
		HTML2COLOR(0x0000b3),         // progress (active area)
	},

	// Pressed color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0xCCCC00),         // edge
		HTML2COLOR(0xCCCC00),         // fill
		HTML2COLOR(0xCCCC00),         // progress (active area)
	}
};

const GWidgetStyle axislabels_active = {
	HTML2COLOR(0x000000),              // background
	HTML2COLOR(0xFFFFFF),              // focus

	// Enabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0xe62e00),         // edge
		HTML2COLOR(0xe62e00),         // fill
		HTML2COLOR(0xe62e00),         // progress (inactive area)
	},

	// Disabled color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0x0000b3),         // edge
		HTML2COLOR(0x0000b3),         // fill
		HTML2COLOR(0x0000b3),         // progress (active area)
	},

	// Pressed color set
	{
		HTML2COLOR(0xFFFFFF),         // text
		HTML2COLOR(0xCCCC00),         // edge
		HTML2COLOR(0xCCCC00),         // fill
		HTML2COLOR(0xCCCC00),         // progress (active area)
	}
};