<?xml version="1.0"?>
<!--
    transform the menu XML file to C code
    Copyright (C) 2012 Boris Boesler

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not sse <http://www.gnu.org/licenses/>.
-->

<xsl:stylesheet version="1.0"
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:xs="http://www.w3.org/2001/XMLSchema">

  <!-- set output mode to text -->
  <xsl:output method="text"
	      indent="no"
	      media-type="text/ascii"/> 


  <xsl:template match="/menu">
    <!-- root itself -->
    /* this file is generated - do not edit */
#include "config.h"
#include "debug.h"
#include "menuitem.h"

    <!-- emit C proto types -->
    <xsl:call-template name="generate_menu_prototype">
      <xsl:with-param name="menu_entry" select="."/>
      <xsl:with-param name="is_root" select="1"/>
    </xsl:call-template>

    <!-- emit C strings in PROGMEM -->
    <xsl:call-template name="generate_progmem_string">
      <xsl:with-param name="menu_entry" select="."/>
    </xsl:call-template>

    <!-- emit C data structures -->
    <xsl:call-template name="generate_menu_entry">
      <xsl:with-param name="menu_entry" select="."/>
      <xsl:with-param name="is_root" select="1"/>
    </xsl:call-template>
  </xsl:template>


  <!--
      emit a C proto type
  -->
  <xsl:template name="generate_menu_prototype">
    <xsl:param name="menu_entry" select="NO_MENU_ENTRY"/>
    <xsl:param name="is_root" select="0"/>
    <xsl:if test="@id">
extern const menu_entry_t menu_<xsl:value-of select="@id"/>[<xsl:value-of select="count(menu)"/>+1];
    </xsl:if>

    <xsl:for-each select="menu">
      <xsl:if test="count(menu)">
	<xsl:call-template name="generate_menu_prototype">
	  <xsl:with-param name="menu_entry" select="."/>
	  <xsl:with-param name="is_root" select="0"/>
	</xsl:call-template>
      </xsl:if>
    </xsl:for-each>

  </xsl:template>


  <!--
      emit a C string in PROGMEM
  -->
  <xsl:template name="generate_progmem_string">
    <xsl:param name="menu_entry" select="NO_MENU_ENTRY"/>
    <xsl:if test="@label">
prog_char menu_label_<xsl:value-of select="@id"/>[] PROGMEM = "<xsl:value-of select="@label"/>";
    </xsl:if>

    <!-- sub menus of this entry -->
    <xsl:for-each select="./menu">
      <xsl:call-template name="generate_progmem_string">
	<xsl:with-param name="menu_entry" select="."/>
      </xsl:call-template>
    </xsl:for-each>

  </xsl:template>


  <!--
      emit a C data structure
  -->
  <xsl:template name="generate_menu_entry">
    <xsl:param name="menu_entry" select="NO_MENU_ENTRY"/>
    <xsl:param name="is_root" select="0"/>
const menu_entry_t menu_<xsl:value-of select="@id"/>[<xsl:value-of select="count(menu)"/>+1] = {
    <xsl:for-each select="menu">
      { menu_label_<xsl:value-of select="@id"/><!--"<xsl:value-of select="@label"/>"-->,

      <!-- left key -->
#ifdef MENU_USE_LEFT_KEY
      <xsl:choose>
	<xsl:when test="@left">
	  <xsl:value-of select="@left"/>
	</xsl:when>
	<xsl:when test="../../@id">
	  menu_<xsl:value-of select="../../@id"/>
	</xsl:when>
	<xsl:otherwise>NO_MENU</xsl:otherwise>
      </xsl:choose>,
#endif

      <!-- right key -->
      <!-- if there is sub-menu then ignore right -->
      <xsl:choose>
	<xsl:when test="count(./menu)">
	  menu_<xsl:value-of select="./@id"/>
	</xsl:when>
	<xsl:otherwise>NO_MENU</xsl:otherwise>
      </xsl:choose>,

      <!-- right function key -->
      <!-- if there is sub-menu then ignore right -->
      <xsl:choose>
	<xsl:when test="count(./menu)">
	  NO_FUNC
	</xsl:when>
	<!--
	    else if there is no right but select then resuse select
	    for right
	-->
	<xsl:otherwise>
	  <xsl:choose>
	    <xsl:when test="@right">
	      <xsl:value-of select="@right"/>
	    </xsl:when>
	    <xsl:otherwise>
	      <xsl:choose>
		<xsl:when test="@select">
		  <xsl:value-of select="@select"/>
		</xsl:when>
		<xsl:otherwise>NO_FUNC</xsl:otherwise>
		</xsl:choose>
	    </xsl:otherwise>
	  </xsl:choose>
	</xsl:otherwise>
	<!--
	<xsl:otherwise>NO_MENU</xsl:otherwise>
	-->
      </xsl:choose>,
      <!--
      NO_FUNC,
      -->

      <!-- select key -->
      <xsl:choose>
	<xsl:when test="@select">
	  <xsl:value-of select="@select"/>
	</xsl:when>
	<xsl:otherwise>NO_FUNC</xsl:otherwise>
      </xsl:choose> },
    </xsl:for-each>
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    <!-- sub menus of this entry -->
    <xsl:for-each select="./menu">
      <xsl:if test="count(./menu)">
	<xsl:call-template name="generate_menu_entry">
	  <xsl:with-param name="menu_entry" select="."/>
	</xsl:call-template>
      </xsl:if>
    </xsl:for-each>

  </xsl:template>

</xsl:stylesheet>
