<?xml version="1.0"?>
<!--
    transform the menu XML file to XML file with entities
    Copyright (C) 2013 Boris Boesler

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

    <xsl:text>&lt;?xml version="1.0" encoding="utf-8"?&gt;
&lt;!--
      this is a generated file, do not edit
--&gt;
    </xsl:text>

    <!-- emit an entity definition -->
    <xsl:call-template name="generate_entity_definition">
      <xsl:with-param name="menu_entry" select="."/>
    </xsl:call-template>

  </xsl:template>


  <!--
      emit an entity definition
  -->
  <xsl:template name="generate_entity_definition">
    <xsl:param name="menu_entry" select="NO_MENU_ENTRY"/>
    <xsl:if test="@label">
&lt;!ENTITY MENU_<xsl:value-of select="@id"/><xsl:text> "&lt;literal&gt;</xsl:text><xsl:value-of select="@label"/><xsl:text>&lt;/literal&gt;"</xsl:text>&gt;
    </xsl:if>

    <xsl:for-each select="menu">
      <xsl:if test="count(.)">
	<xsl:call-template name="generate_entity_definition">
	  <xsl:with-param name="menu_entry" select="."/>
	</xsl:call-template>
      </xsl:if>
    </xsl:for-each>

  </xsl:template>

</xsl:stylesheet>
