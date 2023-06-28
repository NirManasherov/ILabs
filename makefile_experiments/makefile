CC=gcc
CPPFLAGS := -ansi -pedantic-errors -Wall -Wextra -g -Og
TARGETS := fooer.out
RELEASE_FLAGS := -DNDEBUG -O3
DEBUG_FLAG = -g

ifeq '$(findstring release,$(MAKECMDGOALS))' 'release'
$(eval CPPFLAGS += $(RELEASE_FLAGS))
else
$(eval CPPFLAGS += $(DEBUG_FLAG))
endif

fooer.out : foo.o main.o
	$(CC) $(CPPFLAGS) $^ -o $@
	
foo.o : foo.c
	$(CC) $(CPPFLAGS) -c $< -o $@

main.o : main.c
	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: clean all debug
clean:
	-rm -f *.o
	-rm -f *.out
	@echo 'cleaning done!'

all: $(TARGETS)

debug: $(word 1,$(TARGETS))

release: $(word 1,$(TARGETS))
