include common.mk

CFLAGS = -I../include -I../private
CFLAGS += -std=c99 -Wall -Werror -W -pedantic

.PHONY = all check clean

TESTS = insert-sort \
        merge-sort \
		quick-sort

TESTS_OK = $(TESTS:=.ok)

deps = $(TESTS:%:%.o.d)

check: $(TESTS_OK)

NUMBERS = 256 512 1024 2048 4096 8192

$(TESTS_OK): %.ok : %
	$(Q)$(PRINTF) "*** Validating $< ***\n"
	$(Q)./$< && $(PRINTF) "\t$(PASS_COLOR)[ Verified ]$(NO_COLOR)\n"

	@if [ $< = 'merge-sort' ]; then \
		for i in $(NUMBERS); do\
			./$< -s $$i && $(PRINTF) "\t$(PASS_COLOR)[ Verified ]$(NO_COLOR)\n";\
		done;\
	fi

	@touch $@

## standard build rules
.SUFFIXES: .o .c
.c.o:
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF $@.d $<

$(TESTS): % : %.o
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(VECHO) "  Cleaning...\n"
	$(Q)$(RM) $(TESTS) $(TESTS_OK) $(TESTS:=.o) $(TESTS:=.o.d)

-include $(deps)  
