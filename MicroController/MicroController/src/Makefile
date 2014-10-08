#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2013
# 
#############################################################################

# Specify the project name, this is also the main .cpp file to compile
PROJECT	:= 

# Specify target chip or board or system
TARGET	 := lpc1114fn28
	
# Specify the location of the bmptk library
BMPTK		:= ..\..\bmptk-th04

# Specify project files (other than $(PROJECT).cpp) (if any)
OBJECTS		:= 
SOURCES		:= 
HEADERS		:= index.h stats.h toggle.h
LIBRARIES	:= pins timer SPI ARP Ethernet ENC28J60 IP TCP HTTP ADC uart modules i2c
SEARCH		:= ../libraries

# specify (extra) results you want to get (if any)
RESULTS		:= 

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
