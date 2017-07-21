//Show Settings page #1
void showSettings1(void) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// create container widget
	ghContainer = createContainer(wi);
	
	//page configuration (3x2, indexes from left to right, top to bottom):
	
	//labels
	buttonLabels[0] = "Level sensor menu";
	buttonLabels[1] = "Set date";
	buttonLabels[2] = "";
	buttonLabels[3] = "Set time";
	buttonLabels[4] = "Network menu";
	buttonLabels[5] = "";
	
	//images (must be defined under Images above and declared through guiCreate below)
	buttonImages[0] = &_obenlinks;
	buttonImages[1] = &_obenmitte;
	buttonImages[2] = &weiter;
	buttonImages[3] = &_untenlinks;
	buttonImages[4] = &_untenmitte;
	buttonImages[5] = &zurueck;
	
	//functions (must be declared above)
	buttonFunctions[0] = showLevelSensor;
	buttonFunctions[1] = showSetDate;
	buttonFunctions[2] = showSettings2;
	buttonFunctions[3] = showSetTime;
	buttonFunctions[4] = showNetworkMenu;
	buttonFunctions[5] = showMainMenu;
	
	//create the buttons using the information above
	int i;
	for(i = 0; i < 6; i++) {
		createButton(wi, i);
	}
	
	//show the page
	gwinShow(ghContainer);
}

//Show Settings page #2
void showSettings2(void) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// create container widget
	ghContainer = createContainer(wi);
	
	//page configuration (3x2, indexes from left to right, top to bottom):
	
	//labels
	buttonLabels[0] = "Solar power menu";
	buttonLabels[1] = "Factory reset";
	buttonLabels[2] = "";
	buttonLabels[3] = "";
	buttonLabels[4] = "";
	buttonLabels[5] = "";
	
	//images (must be defined under Images above and declared through guiCreate below)
	buttonImages[0] = &_obenlinks;
	buttonImages[1] = &_obenmitte;
	buttonImages[2] = &weiter;
	buttonImages[3] = &_untenlinks;
	buttonImages[4] = &_untenmitte;
	buttonImages[5] = &zurueck;
	
	//functions (must be declared above)
	buttonFunctions[0] = showSolarPower;
	buttonFunctions[1] = showFactoryReset;
	buttonFunctions[2] = showSettings1;
	buttonFunctions[3] = nothing;
	buttonFunctions[4] = nothing;
	buttonFunctions[5] = showMainMenu;
	
	//create the buttons using the information above
	int i;
	for(i = 0; i < 6; i++) {
		createButton(wi, i);
	}
	
	//show the page
	gwinShow(ghContainer);
}

//Show level sensor menu page
void showLevelSensor(void) {
	createAnswer(doLevelSensor, showSettings1, &NumpadRelationKeyboard, "0000/0000");
}

//If the user requested a level sensor adjustment, do an adjustment
void doLevelSensor(void) {
	/* READ TEXTEDIT AND LEVEL SENSOR VALUES HERE */
	
	destroyPage();
	showSettings1();
}


//Show set date page
void showSetDate(void) {
	createAnswer(doSetDate, showSettings1, &NumpadDateKeyboard, "19.07.2017");
}

//If the user requested a date set, do a date set
void doSetDate(void) {
	/* READ TEXTEDIT AND SET DATE VALUE HERE */
	
	destroyPage();
	showSettings1();
}

//Show set time page
void showSetTime(void) {
	createAnswer(doSetTime, showSettings1, &NumpadTimeKeyboard, "15:28");
}

//If the user requested a time set, do a time set
void doSetTime(void) {
	/* READ TEXTEDIT AND SET TIME VALUE HERE */
	
	destroyPage();
	showSettings1();
}

//Show network menu page
void showNetworkMenu(void) {
	createQuestion(doNetworkMenu, showSettings1, "Work in progress...");
}

//If the user requested a network menu, do a network menu
void doNetworkMenu(void) {
	/* DO NETWORK MENU AND SHOW RESPECTIVE PAGES HERE */
	
	showSettings1();
}

//Show solar power menu page
void showSolarPower(void) {
	createQuestion(doSolarPower, showSettings2, "Work in progress...");
}

//If the user requested a solar power menu, do a solar power menu
void doSolarPower(void) {
	/* DO SOLAR POWER MENU AND SHOW RESPECTIVE PAGES HERE */
	
	showSettings2();
}

//Show factory reset page
void showFactoryReset(void) {
	createQuestion(doFactoryReset, showSettings2, "Are you sure you want to perform a factory reset?");
}

//If the user requested a factory reset, do a factory reset
void doFactoryReset(void) {
	/* RESET ALL VALUES HERE */
	
	//dramatic pause
	gfxSleepMilliseconds(2500);
	
	showOverview();
}
