.PHONY: clean All

All:
	@cd "Task1" && "$(MAKE)" -f  "Task1.mk"
clean:
	@cd "Task_2" && "$(MAKE)" -f  "Task2.mk" clean
