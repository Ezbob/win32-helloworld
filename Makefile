SOURCE_DIR=src
BUILD_DIR=build
ROOT_DIR=$(shell pwd)

MODULE?=

C_MODULES=helloapi

C_SOURCE_DIR=${SOURCE_DIR}/c
C_BUILD_DIR=${BUILD_DIR}/c
C_MODULES_DIRS=$(addprefix ${C_SOURCE_DIR}/,${C_MODULES})

debug-all: ${C_MODULES_DIRS}/debug

release-all: ${C_MODULES_DIRS}/release

${C_BUILD_DIR}:
	mkdir -p $@

${C_MODULES_DIRS}/debug: ${C_BUILD_DIR}
	BUILD_DIR_PREFIX=${ROOT_DIR}/${C_BUILD_DIR}/$(notdir $(@D)) ${MAKE} -C $(@D) debug

${C_MODULES_DIRS}/release: ${C_BUILD_DIR}
	BUILD_DIR_PREFIX=${ROOT_DIR}/${C_BUILD_DIR}/$(notdir $(@D)) ${MAKE} -C $(@D) release

run:
ifneq (${MODULE},)
	BUILD_DIR_PREFIX=${ROOT_DIR}/${C_BUILD_DIR}/${MODULE} ${MAKE} -C ${C_SOURCE_DIR}/${MODULE} run
else
	@echo "Error: 'MODULE' is not set"
	@exit 1
endif

clean:
	rm -rf ${BUILD_DIR}