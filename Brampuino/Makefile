
# Create code from XML specifications and program documentation
# Copyright (C) 2012 Boris Boesler
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not sse <http://www.gnu.org/licenses/>.

# tools
XSLTPROC=xsltproc
XSLTPROC_FLAGS= --xinclude --nonet

# files
XML_FILE=menu.xml
XSL_FILE=menu.xsl

SOURCE_FILES= debug.h config.h \
	Brampuino.ino Brampuino.h \
	menuitem.cpp menuitem.h \
	menu.cpp \
	button.cpp button.h \
	lcd.cpp lcd.h \
	camera.cpp camera.h

DOXYGEN_CONFIG = doxygen.config
DOXYGEN_OUTPUT_DIR = doc
DOXYGEN_OUTPUT_FILE = $(DOXYGEN_OUTPUT_DIR)/html/index.html

#
# general targets
#
all:	menu.cpp

clean:
	$(RM) menu.cpp

doc:	$(DOXYGEN_OUTPUT_FILE)

# rules to generate files

$(DOXYGEN_OUTPUT_FILE):	$(DOXYGEN_CONFIG) $(SOURCE_FILES)
	doxygen $(DOXYGEN_CONFIG)

menu.cpp:	$(XML_FILE) $(XSL_FILE)
	$(XSLTPROC) $(XSLTPROC_FLAGS) --output $@ $(XSL_FILE) $<
