CC = gcc
CXX = g++
AR = ar
LD = g++

INC = 
CFLAGS = -Wall `sdl-config --cflags`
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image

PROGRAM_NAME = grammagic
INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/debug
OUTDIR_DEBUG = bin/debug
OUT_DEBUG = $(OUTDIR_DEBUG)/$(PROGRAM_NAME)

OBJ_DEBUG = $(OBJDIR_DEBUG)/res/strings.o $(OBJDIR_DEBUG)/res/templates.o $(OBJDIR_DEBUG)/res/battleCommands.o $(OBJDIR_DEBUG)/view/menuViewManager.o $(OBJDIR_DEBUG)/view/viewManagerMenuToolsBase.o $(OBJDIR_DEBUG)/view/viewManager.o $(OBJDIR_DEBUG)/view/victoryViewManager.o $(OBJDIR_DEBUG)/view/titleViewManager.o $(OBJDIR_DEBUG)/view/statsViewManager.o $(OBJDIR_DEBUG)/view/miniMapViewManager.o $(OBJDIR_DEBUG)/view/mapViewManager.o $(OBJDIR_DEBUG)/view/controlViewManager.o $(OBJDIR_DEBUG)/util/utils.o $(OBJDIR_DEBUG)/util/events.o $(OBJDIR_DEBUG)/util/assetCache.o $(OBJDIR_DEBUG)/title/titleStateManager.o $(OBJDIR_DEBUG)/stateManager.o $(OBJDIR_DEBUG)/magic/spell.o $(OBJDIR_DEBUG)/menuManager.o $(OBJDIR_DEBUG)/menuItem.o $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/magic/word.o $(OBJDIR_DEBUG)/magic/verb.o $(OBJDIR_DEBUG)/magic/targetAll.o $(OBJDIR_DEBUG)/magic/spellContext.o $(OBJDIR_DEBUG)/magic/rune.o $(OBJDIR_DEBUG)/magic/noun.o $(OBJDIR_DEBUG)/magic/modifier.o $(OBJDIR_DEBUG)/magic/command.o $(OBJDIR_DEBUG)/magic/auxilliary.o $(OBJDIR_DEBUG)/magic/adverb.o $(OBJDIR_DEBUG)/play/mapCell.o $(OBJDIR_DEBUG)/play/terrain.o $(OBJDIR_DEBUG)/play/playStateManager.o $(OBJDIR_DEBUG)/play/pc.o $(OBJDIR_DEBUG)/play/party.o $(OBJDIR_DEBUG)/play/npc.o $(OBJDIR_DEBUG)/play/mob.o $(OBJDIR_DEBUG)/play/mapObject.o $(OBJDIR_DEBUG)/play/jobClass.o $(OBJDIR_DEBUG)/play/gameMap.o $(OBJDIR_DEBUG)/play/enemy.o $(OBJDIR_DEBUG)/play/combatManager.o $(OBJDIR_DEBUG)/play/battleField.o $(OBJDIR_DEBUG)/persistence/saveLoad.o

all: debug

clean: clean_debug

ensure_folders:
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OUTDIR_DEBUG) || mkdir -p $(OUTDIR_DEBUG)
	test -d $(OUTDIR_DEBUG)/maps || mkdir -p $(OUTDIR_DEBUG)/maps
	test -d $(OUTDIR_DEBUG)/images || mkdir -p $(OUTDIR_DEBUG)/images
	test -d $(OUTDIR_DEBUG)/fonts || mkdir -p $(OUTDIR_DEBUG)/fonts
	test -d $(OBJDIR_DEBUG)/res || mkdir -p $(OBJDIR_DEBUG)/res
	test -d $(OBJDIR_DEBUG)/view || mkdir -p $(OBJDIR_DEBUG)/view
	test -d $(OBJDIR_DEBUG)/util || mkdir -p $(OBJDIR_DEBUG)/util
	test -d $(OBJDIR_DEBUG)/title || mkdir -p $(OBJDIR_DEBUG)/title
	test -d $(OBJDIR_DEBUG)/magic || mkdir -p $(OBJDIR_DEBUG)/magic
	test -d $(OBJDIR_DEBUG)/play || mkdir -p $(OBJDIR_DEBUG)/play
	test -d $(OBJDIR_DEBUG)/persistence || mkdir -p $(OBJDIR_DEBUG)/persistence

debug: ensure_folders resources_debug $(PROGRAM_NAME)_debug

resources_debug:
	cp res/images/* $(OUTDIR_DEBUG)/images
	cp res/maps/* $(OUTDIR_DEBUG)/maps
	cp res/fonts/* $(OUTDIR_DEBUG)/fonts

$(PROGRAM_NAME)_debug: ensure_folders $(OBJ_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/%.o: %.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $^ -o $@

clean_debug:
	rm -rf $(OUTDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)

.PHONY: ensure_folders clean_debug

