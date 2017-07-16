DEMODIR = $(GFXLIB)/demos/games/tetris


GFXINC +=   $(DEMODIR)
GFXINC +=   .

GFXSRC +=	$(DEMODIR)/main.c \
			$(DEMODIR)/tetris.c
