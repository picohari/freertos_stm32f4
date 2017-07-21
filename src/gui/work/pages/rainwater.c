//Show Rainwater Filtration page
void showRainwater(void) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// create container widget
	ghContainer = createContainer(wi);
	
	//page configuration (3x2, indexes from left to right, top to bottom):
	
	//labels
	buttonLabels[0] = "Membranes config";
	buttonLabels[1] = "C1/C2 suction";
	buttonLabels[2] = "";
	buttonLabels[3] = "Chemical clean";
	buttonLabels[4] = "Test mode menu";
	buttonLabels[5] = "";
	
	//images (must be defined under Images above and declared through guiCreate below)
	buttonImages[0] = &_obenlinks;
	buttonImages[1] = &_obenmitte;
	buttonImages[2] = &_obenrechts;
	buttonImages[3] = &_untenlinks;
	buttonImages[4] = &_untenmitte;
	buttonImages[5] = &zurueck;
	
	//functions (must be declared above)
	buttonFunctions[0] = showMembranes;
	buttonFunctions[1] = showC12Suction;
	buttonFunctions[2] = nothing;
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

//Show membranes page
void showMembranes(void) {
	createAnswer(doMembranes, showRainwater, &NumpadKeyboard, "1");
}

//If the user requested a membranes set, do a set
void doMembranes(void) {
	/* READ TEXTEDIT AND SET MEMBRANES VALUE HERE */
	gwinGetText(ghTextedit);
	
	showRainwater();
}

//Show C1/C2 suction page
void showC12Suction(void) {
	createAnswer(doC12Suction, showRainwater, &NumpadRelationKeyboard, "0/0");
}

//If the user requested a C1/C2 suction set, do a set
void doC12Suction(void) {
	/* READ TEXTEDIT AND SET MEMBRANES VALUE HERE */
	gwinGetText(ghTextedit);
	
	showRainwater();
}

//Show chemical clean page
void showChemicalClean(void) {
	createQuestion(doChemicalClean, showRainwater, "Are you sure you want to perform a chemical clean?");
}

//If the user requested a chemical clean, do a chemical clean
void doChemicalClean(void) {
	/* DO CHEMICAL CLEAN AND SHOW RESPECTIVE PAGES HERE */
	
	showRainwater();
}

//Show chemical clean page
void showTestMode(void) {
	createYesNo(doTestMode, showRainwater);
}

//If the user requested a chemical clean, do a chemical clean
void doTestMode(void) {
	/* OPEN TEST MODE AND SHOW RESPECTIVE PAGES HERE */
	
	showRainwater();
}
