//Show Main Menu page
void showMainMenu(void) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// create container widget
	ghContainer = createContainer(wi);
	
	//page configuration (3x2, indexes from left to right, top to bottom):
	
	//labels (as strings)
	buttonLabels[0] = "Rainwater Filtration";
	buttonLabels[1] = "Infiltration";
	buttonLabels[2] = "";
	buttonLabels[3] = "Greywater Treatment";
	buttonLabels[4] = "Fire Protection Tank";
	buttonLabels[5] = "";
	
	//images (must be defined through guiCreate below)
	buttonImages[0] = &_obenlinks;
	buttonImages[1] = &_obenmitte;
	buttonImages[2] = &einstellungen;
	buttonImages[3] = &_untenlinks;
	buttonImages[4] = &_untenmitte;
	buttonImages[5] = &zurueck;
	
	//functions (must be defined above)
	buttonFunctions[0] = showRainwater;
	buttonFunctions[1] = showInfiltration;
	buttonFunctions[2] = showSettings1;
	buttonFunctions[3] = showGreywater;
	buttonFunctions[4] = showFireProtection;
	buttonFunctions[5] = showOverview;
	
	//create the buttons using the information above
	int i;
	for(i = 0; i < 6; i++) {
		createButton(wi, i);
	}
	
	//show the page
	gwinShow(ghContainer);
}
