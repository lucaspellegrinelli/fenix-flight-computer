filenames := controller
files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

all:
	g++ controller.cpp Matrix.cpp Matrix.h -o controller

clean:
ifneq ($(files),)
	rm -f $(files)
endif
