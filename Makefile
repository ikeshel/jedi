#
# ? -> i.keshelashvili@fz-juelich.de
#

all:
	@echo $(shell root -n -l -b -q macros/make.C)
	@echo 
	@echo " - - - compilation done!!! - - -\n"

docs:
	@echo $(shell root -b -q macros/docs.C)

clean:
	@rm -r -v -f lib

