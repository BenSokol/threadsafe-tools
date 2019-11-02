# @Filename: Makefile-Settings.mk
# @Author:   Ben Sokol <Ben>
# @Email:    ben@bensokol.com
# @Created:  February 14th, 2019 [5:21pm]
# @Modified: November 1st, 2019 [6:30pm]
# @Version:  1.0.0
#
# Copyright (C) 2019 by Ben Sokol. All Rights Reserved.

###############################################################################
# Makefile Settings                                                           #
###############################################################################

EXE=ts-benchmark
TESTEXE=ts-test

CC=gcc-9
CXX=g++-9

CCSTD = -std=c11
CXXSTD = -std=c++2a

INC = -Iexternal/Catch2/single_include/catch2
WARNING_FLAGS += -Wall -Wextra -Wpedantic
WARNING_FLAGS_GCC += -Wno-clobbered
WARNING_FLAGS_CLANG += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-date-time -Wno-exit-time-destructors -Wno-global-constructors -Wno-disabled-macro-expansion -Wno-switch-enum -Wno-unreachable-code-break -Wno-unreachable-code-return -Wno-missing-declarations -Wno-missing-variable-declarations

MACOS_FLAGS +=
LINUX_FLAGS += -pthread

CPPCHECK_SUPPRESS = --suppress=missingIncludeSystem --suppress=preprocessorErrorDirective:submodules/utilities/UTL_textWrap.cpp:18 --suppress=preprocessorErrorDirective:submodules/debug/DBG_out.hpp:36
CPPCHECK_FLAGS = -q --platform=native --error-exitcode=1 --force
CPPCHECK_ENABLE = --enable=warning --enable=style --enable=missingInclude --enable=performance --enable=portability
