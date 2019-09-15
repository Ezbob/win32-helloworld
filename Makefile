SOURCE_DIR=src
BUILD_DIR=build
ROOT_DIR=$(shell pwd)
MODULE?=

C_MODULES?=
C_SOURCE_DIR=${SOURCE_DIR}/c
C_BUILD_DIR=${BUILD_DIR}/c
C_MODULES_DIRS=$(addprefix ${C_SOURCE_DIR}/,${C_MODULES})

include modules.mk

debug: _check_module ${C_BUILD_DIR}
	BUILD_DIR_PREFIX=$(ROOT_DIR)/${C_BUILD_DIR}/${MODULE} ${MAKE} -C ${C_SOURCE_DIR}/${MODULE} debug

release: _check_module ${C_BUILD_DIR}
	BUILD_DIR_PREFIX=$(ROOT_DIR)/${C_BUILD_DIR}/${MODULE} ${MAKE} -C ${C_SOURCE_DIR}/${MODULE} release

run: _check_module ${C_BUILD_DIR}
	BUILD_DIR_PREFIX=${ROOT_DIR}/${C_BUILD_DIR}/${MODULE} ${MAKE} -C ${C_SOURCE_DIR}/${MODULE} run

clean: _check_module ${C_BUILD_DIR}
	rm -rf ${ROOT_DIR}/${C_BUILD_DIR}/${MODULE}

debug-all: ${C_MODULES_DIRS}/debug

release-all: ${C_MODULES_DIRS}/release

clean-all:
	rm -rf ${BUILD_DIR}

${C_BUILD_DIR}:
	mkdir -p $@

${C_MODULES_DIRS}/debug: ${C_BUILD_DIR}
	BUILD_DIR_PREFIX=${ROOT_DIR}/${C_BUILD_DIR}/$(notdir $(@D)) ${MAKE} -C $(@D) debug

${C_MODULES_DIRS}/release: ${C_BUILD_DIR}
	BUILD_DIR_PREFIX=${ROOT_DIR}/${C_BUILD_DIR}/$(notdir $(@D)) ${MAKE} -C $(@D) release

_check_module:
ifeq (${MODULE},)
	$(error "Error: 'MODULE' is not set")
endif