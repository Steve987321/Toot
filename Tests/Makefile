# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=release
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),release)
  gtest_config = release
  Tests_config = release

else ifeq ($(config),debug)
  gtest_config = debug
  Tests_config = debug

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := gtest Tests

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

gtest:
ifneq (,$(gtest_config))
	@echo "==== Building gtest ($(gtest_config)) ===="
	@${MAKE} --no-print-directory -C . -f gtest.make config=$(gtest_config)
endif

Tests: gtest
ifneq (,$(Tests_config))
	@echo "==== Building Tests ($(Tests_config)) ===="
	@${MAKE} --no-print-directory -C . -f Tests.make config=$(Tests_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f gtest.make clean
	@${MAKE} --no-print-directory -C . -f Tests.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  release"
	@echo "  debug"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   gtest"
	@echo "   Tests"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"