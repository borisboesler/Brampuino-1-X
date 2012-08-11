#!/bin/sh

# create a docbook [pub]date
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

PROGRAM=$0
PUBDATE="no"
DATETAG="date"

#
# usage
#
usage() {
cat <<- _EOF_
$PROGRAM echos last git checkin time as a DocBook[pub]date entry
(c) 2012 Boris Boesler
License: GPLv2
usage: $PROGRAM [options]
possible options are:
-h | --help             print this help message
--pubdate               emit date a a pubdate
all other options are ignored
_EOF_
}


#
# analyze arguments
#
analyze_arguments() {
  while [ $# -gt 0 ]; do
    case $1 in
	-h|--help)           usage;;
	--pubdate)           PUBDATE="yes";;
	*)                   echo "Unknown option "$1;;
    esac
    shift
  done
}


#
# emit
#
emit() {
    if [ "yes" == $PUBDATE ] ; then
	DATETAG="pubdate"
    fi
    DATEVAL=`git log -n 1 --format=%cD`
    # header
    echo '<?xml version="1.0" encoding="UTF-8"?>'
    echo '<!DOCTYPE '$DATETAG' PUBLIC "-//OASIS//DTD DocBook XML V4.5//EN"'
    echo '          "http://www.docbook.org/xml/4.5/docbookx.dtd">'
    echo '<'$DATETAG'>'$DATEVAL'</'$DATETAG'>'
}


#
#
#
analyze_arguments $*
emit
