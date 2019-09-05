filenames := controller
files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

all:
	g++ controller.cpp -o controller

clean:
ifneq ($(files),)
	rm -f $(files)
endif
