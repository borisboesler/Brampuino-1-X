
# Create code and DocBook files from XML specifications
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

#
#

# current version
B1X_MAJOR = 0
B1X_MINOR = 2
B1X_PATCHLEVEL = 6

MAKE_VERSION_STR=B1X_MAJOR=$(B1X_MAJOR) B1X_MINOR=$(B1X_MINOR) B1X_PATCHLEVEL=$(B1X_PATCHLEVEL)

# tools

# files

#
# general targets
#
all:	make_in_source make_in_manual

make_in_source:
	$(MAKE) $(MAKE_FLAGS) $(MAKE_VERSION_STR) -C Brampuino

make_in_manual:
	$(MAKE) $(MAKE_FLAGS) $(MAKE_VERSION_STR) -C manual
