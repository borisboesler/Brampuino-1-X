<?xml version="1.0" encoding="UTF-8"?>

<!--
DocBook XML -> FO
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

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version="1.0">

  <!-- IMPORT the default templates -->
  <xsl:import href="http://docbook.sourceforge.net/release/xsl/current/fo/docbook.xsl"/>

  <!-- INCLUDE the nicer ones and over write the old ones -->
  <!-- xsl:include href="mydocbook.xsl"/ -->

  <!-- or look at
  http://www.sagehill.net/docbookxsl/HTMLTitlePage.html#TitlePageElems
  and use CSS
  -->


  <!--
      **
      ** very general for FOP and HTML
      ** should be in common file
      **
  -->

  <!-- bibliography numbering: no -->
  <xsl:param name="bibliography.numbered">1</xsl:param>
  
  <!-- xrefs with title: no -->
  <xsl:param name="xref.with.number.and.title">0</xsl:param>



  <!-- set bigger font size -->
  <xsl:attribute-set name="root.properties">
    <xsl:attribute name="font-size">10</xsl:attribute>
  </xsl:attribute-set>

  <!--
      body
  -->
  <!-- fonts and sizes -->
  <xsl:param name="body.font.size">11</xsl:param>
  <xsl:param name="body.font.family">Helvetica</xsl:param>
  <xsl:param name="body.font.master">11</xsl:param>
  <!-- page body indention -->
  <xsl:param name="body.start.indent">0</xsl:param>

  <!--
      titles
  -->
  <xsl:param name="title.font.family">Times Roman</xsl:param>
  <xsl:param name="title.margin.left">0in</xsl:param>

  <!--
      sections
  -->
  <xsl:param name="section.autolabel">1</xsl:param>
  <xsl:param name="section.label.includes.component.label">1</xsl:param>


  <!--
  -->
  <xsl:param name="footnote.font.size">9</xsl:param>

  <!-- page header layout -->
  <xsl:param name="header.column.widths">1 3 1</xsl:param>


  <!-- use SVG graphics in print/PDFs -->
  <xsl:param name="callout.graphics.extension">.svg</xsl:param>


  <!-- backgrounds -->
  <xsl:attribute-set name="admonition.properties">
    <xsl:attribute name="background-color">#dddddd</xsl:attribute>
  </xsl:attribute-set>

  <xsl:attribute-set name="example.properties">
    <xsl:attribute name="background-color">#eeeeee</xsl:attribute>
  </xsl:attribute-set>

  <xsl:attribute-set name="verbatim.properties">
    <xsl:attribute name="background-color">#dddddd</xsl:attribute>
  </xsl:attribute-set>

  <xsl:template name="table.cell.block.properties">
    <xsl:if test="ancestor::thead">
      <xsl:attribute name="background-color">#dddddd</xsl:attribute>
    </xsl:if>
  </xsl:template>

  <!--
      rewrite some file reference stuff
      http://lists.oasis-open.org/archives/docbook-apps/200511/msg00071.html
  -->
  <xsl:template match="@fileref">
    <xsl:value-of select="."/>
  </xsl:template>

  <!-- @@@ HINT not xi:  -->
  <xsl:include href="brampuino-titlepage.xsl"/>

</xsl:stylesheet>
