bin_PROGRAMS =	mpm
SUBDIRS =		../libs/curl-7.44.0 ../libs/libarchive-3.1.2/
mpm_SOURCES =	Download.cpp \
				Error.cpp \
				Exec.cpp \
				functions.cpp \
				main.cpp \
				Manager.cpp \
				Options.cpp \
				Package.cpp \
				jsoncpp.cpp
AM_CXXFLAGS =	-Wall -Wextra -Werror -std=c++0x -g -I../inc/ -I../libs/curl-7.44.0/include/curl/ -I../libs/json/ -I../libs/libarchive-3.1.2/libarchive/ -L../libs/curl-7.44.0/lib/.libs/ -L../libs/libarchive-3.1.2/.libs/
mpm_LDADD = -lcurl -larchive
