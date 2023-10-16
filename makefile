COMPILER = gcc
PROGRAM  = ./main.c
RESULT   = cheat.exe

$(RESULT): $(PROGRAM)
	$(COMPILER) -g $< -o $@ -lgdi32
	@echo success build

clean:
	@if exist $(RESULT) del $(RESULT)
	@echo success clean
