//Show Greywater Treatment page
void showGreywater(void) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// create container widget
	ghContainer = createContainer(wi);
	
	//page configuration (3x2, indexes from left to right, top to bottom):
	
	//labels
	buttonLabels[0] = "Suction overflow";
	buttonLabels[1] = "Sludge pump";
	buttonLabels[2] = "";
	buttonLabels[3] = "T1/T2 starting times";
	buttonLabels[4] = "Air on/off timer";
	buttonLabels[5] = "";
	
	//images (must be defined under Images above and declared through guiCreate below)
	buttonImages[0] = &_obenlinks;
	buttonImages[1] = &_obenmitte;
	buttonImages[2] = &_obenrechts;
	buttonImages[3] = &_untenlinks;
	buttonImages[4] = &_untenmitte;
	buttonImages[5] = &zurueck;
	
	//functions (must be declared above)
	buttonFunctions[0] = showSuctionOverflow;
	buttonFunctions[1] = showSludgePump;
	buttonFunctions[2] = nothing;
	buttonFunctions[3] = showT12Time;
	buttonFunctions[4] = showAirTimer;
	buttonFunctions[5] = showMainMenu;
	
	//create the buttons using the information above
	int i;
	for(i = 0; i < 6; i++) {
		createButton(wi, i);
	}
	
	//show the page
	gwinShow(ghContainer);
}

//Show suction overflow page
void showSuctionOverflow(void) {
	createAnswer(doSuctionOverflow, showGreywater, &NumpadKeyboard, "0");
}

//If the user requested a suction overflow S0 set, do an S0 set
void doSuctionOverflow(void) {
	/* READ TEXTEDIT AND SET SUCTION OVERFLOW WEEK VALUE HERE */
	
	showGreywater();
}

//Show slude pump page
void showSludgePump(void) {
	createAnswer(doSludgePump, showGreywater, &NumpadRelationKeyboard, "0/0");
}

//If the user requested a slude pump relation set, do a relation set
void doSludgePump(void) {
	/* READ TEXTEDIT AND SET SLP-TIME RELATION VALUE HERE */
	
	showGreywater();
}

//Show T1/T2 time page
void showT12Time(void) {
	createAnswer(doT12Time, showGreywater, &NumpadTimeKeyboard, "09:00");
}

//If the user requested a T1/T2 starting time set, do a time set
void doT12Time(void) {
	/* READ TEXTEDIT AND SET T1/T2 TIME VALUES HERE */
	
	showGreywater();
}

//Show Air on/off timer page
void showAirTimer(void) {
	createAnswer(doAirTimer, showGreywater, &NumpadRelationKeyboard, "0/0");
}

//If the user requested a T1/T2 starting time set, do a time set
void doAirTimer(void) {
	/* READ TEXTEDIT AND SET AIR ON/OFF VALUES HERE */
	
	showGreywater();
}
