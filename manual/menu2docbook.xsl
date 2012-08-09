<?xml version="1.0"?>
<!--
create a DocBook file from menu XML file
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
  <xsl:output method="xml"
	      indent="yes"
	      media-type="text/xml"
	      doctype-public="-//OASIS//DTD DocBook XML V4.5//EN"
	      doctype-system="http://www.oasis-open.org/docbook/xml/4.5/docbookx.dtd"/>
  
  <xsl:template match="/menu">
    <xsl:comment>
      this is a generated file, do not edit
    </xsl:comment>

    <!-- emit variable list for each top level entry -->
    <xsl:call-template name="generate_variablelist">
      <xsl:with-param name="menu_entry" select="."/>
    </xsl:call-template>
  </xsl:template>


  <!--
      emit varlistentry for each top level entry
  -->
  <xsl:template name="generate_variablelist">
    <xsl:param name="menu_entry" select="NO_MENU_ENTRY"/>

    <variablelist>
      <xsl:for-each select="menu">

	<varlistentry>
	  <term><xsl:value-of select="@label"/></term>

	  <listitem>
	    <!-- print doc -->
	    <xsl:choose>
	      <xsl:when test="doc">
		<xsl:copy-of select="doc/*"/>
	      </xsl:when>
	      <xsl:otherwise>
		<para>
		  <xsl:value-of select="@id"/>
		</para>
	      </xsl:otherwise>
	    </xsl:choose>
	    
	    <!-- print sub menu in current listitem -->
	    <xsl:choose>
	      <xsl:when test="0 = count(menu)">
		<xsl:call-template name="generate_varlist_entry">
		  <xsl:with-param name="menu_entry" select="."/>
		</xsl:call-template>
	      </xsl:when>
	      <xsl:when test="0 &lt; count(menu)">
		<xsl:call-template name="generate_variablelist">
		  <xsl:with-param name="menu_entry" select="."/>
		</xsl:call-template>
	      </xsl:when>
	    </xsl:choose>
	    
	  </listitem>
	  </varlistentry>

      </xsl:for-each>
      
    </variablelist>

  </xsl:template>


  <!--
      emit varlistentry for each top level entry
  -->
  <xsl:template name="generate_varlist_entry">
    <xsl:param name="menu_entry" select="NO_MENU_ENTRY"/>
    <xsl:for-each select="menu">
      <xsl:if test="count(menu)">
	<xsl:call-template name="generate_variablelist">
	  <xsl:with-param name="menu_entry" select="."/>
	</xsl:call-template>
      </xsl:if>
    </xsl:for-each>

  </xsl:template>

</xsl:stylesheet>
