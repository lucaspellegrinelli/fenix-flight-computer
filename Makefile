filenames := controller
files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

all:
	g++ controller.cpp matrix/Matrix.cpp matrix/Matrix.h -o controller

clean:
ifneq ($(files),)
	rm -f $(files)
endif
