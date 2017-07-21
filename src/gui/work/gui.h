#ifndef _GUI_H_
#define _GUI_H_

#include "gfx.h"
#include "src/gwin/gwin_keyboard_layout.h"

// Widget groups
#define GROUP_DEFAULT 0

// GListeners
extern GListener glistener;

// GHandles
extern GHandle ghContainer;
extern GHandle ghKeyboard;
extern GHandle ghTextedit;
extern GHandle ghLabel;

// Fonts
extern font_t dejavu_sans_16;

// Button arrays
extern double buttonDimensions[6][4];
extern char *buttonLabels[6];
extern gdispImage *buttonImages[6];
extern void (*buttonFunctions[6])(void);

// Images
extern gdispImage intewa;
extern gdispImage _leer;
extern gdispImage _obenlinks;
extern gdispImage _obenmitte;
extern gdispImage _obenrechts;
extern gdispImage _untenlinks;
extern gdispImage _untenmitte;
extern gdispImage _untenrechts;
extern gdispImage einstellungen;
extern gdispImage zurueck;
extern gdispImage weiter;
extern gdispImage bestaetigen;

// Keyboard Layouts
extern const GVKeyTable QwertzKeyboard;
extern const GVKeyTable NumpadTimeKeyboard;
extern const GVKeyTable NumpadDateKeyboard;
extern const GVKeyTable NumpadRelationKeyboard;
extern const GVKeyTable NumpadKeyboard;

// Function Prototypes
void destroyPage(void);
void nothing(void);

void createButton(GWidgetInit, int);
GHandle createContainer(GWidgetInit);
GWidgetInit createYesNo(void (*)(void), void (*)(void));
void createQuestion(void (*)(void), void (*)(void), char*);
void createAnswer(void (*)(void), void (*)(void), const GVKeyTable*, char*);

void guiCreate(void);
void guiShowPage(unsigned pageIndex);
void guiEventLoop(void);

#endif /* _GUI_H_ */
