//Show Infiltration page
void showInfiltration(void) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// create container widget
	ghContainer = createContainer(wi);
	
	//page configuration (3x2, indexes from left to right, top to bottom):
	
	//labels
	buttonLabels[0] = "";
	buttonLabels[1] = "";
	buttonLabels[2] = "";
	buttonLabels[3] = "";
	buttonLabels[4] = "";
	buttonLabels[5] = "";
	
	//images (must be defined under Images above and declared through guiCreate below)
	buttonImages[0] = &_obenlinks;
	buttonImages[1] = &_obenmitte;
	buttonImages[2] = &_obenrechts;
	buttonImages[3] = &_untenlinks;
	buttonImages[4] = &_untenmitte;
	buttonImages[5] = &zurueck;
	
	//functions (must be declared above)
	buttonFunctions[0] = nothing;
	buttonFunctions[1] = nothing;
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
