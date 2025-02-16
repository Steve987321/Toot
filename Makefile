# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  TVM_config = debug
  Compiler_config = debug

else ifeq ($(config),release)
  TVM_config = release
  Compiler_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := TVM Compiler

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

TVM:
ifneq (,$(TVM_config))
	@echo "==== Building TVM ($(TVM_config)) ===="
	@${MAKE} --no-print-directory -C TVM -f Makefile config=$(TVM_config)
endif

Compiler: TVM
ifneq (,$(Compiler_config))
	@echo "==== Building Compiler ($(Compiler_config)) ===="
	@${MAKE} --no-print-directory -C Compiler -f Makefile config=$(Compiler_config)
endif

clean:
	@${MAKE} --no-print-directory -C TVM -f Makefile clean
	@${MAKE} --no-print-directory -C Compiler -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   TVM"
	@echo "   Compiler"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"