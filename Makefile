.PHONY: clean All

All:
	@echo "----------Building project:[ Task2 - Release ]----------"
	@cd "Task_2" && "$(MAKE)" -f  "Task2.mk"
clean:
	@echo "----------Cleaning project:[ Task2 - Release ]----------"
	@cd "Task_2" && "$(MAKE)" -f  "Task2.mk" clean
