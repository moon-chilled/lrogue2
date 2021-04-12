ROGUE_OBJ = src/hit.o src/init.o src/instruct.o src/inventory.o src/level.o src/machdep.o src/main.o \
    src/message.o src/monster.o src/move.o src/object.o src/pack.o src/play.o src/random.o src/ring.o \
    src/room.o src/save.o src/score.o src/spec_hit.o src/throw.o src/trap.o src/use.o src/zap.o

CC ?= cc

CFLAGS = -Wno-constant-conversion -DUNIX -std=c99 -g3 -ggdb -Iinclude
LDFLAGS = -lcurses


lrogue2: $(ROGUE_OBJ)
	$(CC) -o lrogue2 $(ROGUE_OBJ) $(LDFLAGS)

clean:
	-rm -f src/*.o lrogue2

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
