# -*- mode: makefile-gmake -*-

OS = $(shell uname)
ARCH = $(shell uname -m)

USEGCC = 0
USECLANG = 1

CXXFLAGS-add += -Wall -O3 -ffast-math -std=c++11

ifeq ($(USECLANG),1)
USEGCC = 0
CXX = clang++
endif

ifeq ($(USEGCC),1)
CXX = g++
endif

ifeq ($(OS), Linux)
SHLIB_EXT = so
CXXFLAGS-add+=-fPIC
endif

ifeq ($(OS), FreeBSD)
SHLIB_EXT = so
CXXFLAGS-add+=-fPIC
endif

ifeq ($(OS), OpenBSD)
SHLIB_EXT = so
CXXFLAGS-add+=-fPIC
endif

ifeq ($(OS), Darwin)
SHLIB_EXT = dylib
CXXFLAGS-add+=-fPIC
endif

ifeq ($(OS), WINNT)
SHLIB_EXT = dll
endif

ifeq ($(OS), MINGW64_NT-10.0)
SHLIB_EXT = dll
endif

ifeq ($(OS), MINGW32_NT-10.0-WOW)
SHLIB_EXT = dll
endif

# SunOS with gcc
ifeq ($(OS), SunOS)
SHLIB_EXT = so
CXXFLAGS-add+=-fPIC
endif
