# Hey Emacs, this is a -*- Makefile -*- file
# Project:     Brampuino 1-X
# File name:   doc/Makefile
# Purpose:     documentaion Makefile
# Author:      Boris Boesler
# Modified by: 
# Created:     30.07.2012
# Copyright:   (c) 2012 Boris Boesler
# Licence:     
# CVS-ID:      $Id: Makefile 22 2012-08-08 08:35:41Z boesler $
#

# tools
XSLTPROC=xsltproc
XSLTPROC_FLAGS=--nonet --xinclude

CP=cp
FOP=fop

# files
CATALOG_FILE = catalog.xml
SOURCES = Makefile $(CATALOG_FILE).in usersmanual.db.xml
XML_FILES = $(filter %.xml,$(SOURCES))
FO_FILES = $(XML_FILES:.xml=.fo)
PDF_FILES = $(XML_FILES:.xml=.pdf)
HTML_FILES = $(XML_FILES:.xml=.html)
# ebook
EPUB_FILES = $(XML_FILES:.xml=.epub)
DB2EPUB_DIR = OEBPS
DB2EPUB_IMAGE_DIR = $(DB2EPUB_DIR)/images
DB2EPUB_ICONS_DIR = $(DB2EPUB_DIR)/images/icons
# our own titlepage
# spec template
DOCBOOK_FO_TITLEPAGE_TEMPLATE_SPEC = fo/titlepage.spec.xml
# generated title-page specification
DOCBOOK_FO_TITLEPAGE_TEMPLATE = fo/brampuino-titlepage.xsl
# used stylesheet
DOCBOOK_FO_STYLESHEET = fo/stylesheet.xsl

# menu XML as SVG
#MENU_XML_FILE = ../Brampuino/menu.xml
#MENU_SVG_FILE = menu.svg

# menu XML as DocBook XML
MENU_DOCBOOK_FILE = menu.db.xml

# publication date as DocBook XML
DATE_FILE=date.db.xml
DATE_FILE_CREATOR=./git-date.xml.sh
PUBDATE_FILE=pubdate.db.xml
PUBDATE_FILE_CREATOR=./git-date.xml.sh

# used menufile
MENU_FILE = $(MENU_DOCBOOK_FILE)

CLEANFILES = $(PDF_FILES) $(HTML_FILES)


#
# targets
#

all:	$(CATALOG_FILE) $(PDF_FILES) $(HTML_FILES) $(EPUB_FILES) $(MENU_FILE)

clean:
	$(RM) $(CATALOG_FILE) $(PDF_FILES) $(HTML_FILES) $(FO_FILES)
	$(RM) $(EPUB_FILES) mimetype $(DOCBOOK_FO_TITLEPAGE_TEMPLATE)
	$(RM)  $(MENU_FILE) $(PUBDATE_FILE) $(DATE_FILE)
	$(RM) -r $(DB2EPUB_DIR) META-INF


# docbook system files
ifeq ($(shell uname -s), Linux)
 DOCBOOK_XML_DIR=/usr/share/xml/docbook/schema/dtd
 DOCBOOK_XSL_DIR=/usr/share/xml/docbook/stylesheet/nwalsh
else
 # on a Mac
 DOCBOOK_XML_DIR=/opt/local/share/xml/docbook
 DOCBOOK_XSL_DIR=/opt/local/share/xsl/docbook-xsl-ns
endif

# fo style-sheet
#DOCBOOK_FO_STYLESHEET=stylesheet.xsl

# epub stylesheet
DOCBOOK_EPUB_STYLESHEET=$(DOCBOOK_XSL_DIR)/epub/docbook.xsl


#
# parameters
#
DOCBOOK_DEFAULT_PARAMS= \
	--stringparam admon.graphics 1 \
	--stringparam admon.graphics.path $(DOCBOOK_XSL_DIR)/images/ \
	--stringparam admon.graphics.extension .svg \
	--stringparam callout.graphics.path $(DOCBOOK_XSL_DIR)/images/callouts/

DOCBOOK_HTML_PARAMS=\
	--stringparam html.stylesheet default.css \
	--stringparam bibliography.numbered 1 \
	--stringparam section.autolabel 1 \
	--stringparam section.label.includes.component.label 1

DOCBOOK_FOP_PARAMS=\
	--stringparam fop.extensions 0 \
	--stringparam fop1.extensions 1 \
	--stringparam paper.type A4 \
	--stringparam double.sided 1

      # todo: add addintional parameters, trailing '/' is needed
DOCBOOK_EPUB_PARAMS=\
      --stringparam admon.graphics 1 				\
      --stringparam admon.graphics.extension .png 		\
      --stringparam admon.graphics.path images/icons/ 		\
      --stringparam callout.graphics.extension .png 		\
      --stringparam callout.graphics.path images/icons/callouts/

#
# targets
#

$(DOCBOOK_FO_TITLEPAGE_TEMPLATE):	$(DOCBOOK_FO_TITLEPAGE_TEMPLATE_SPEC)
	env XML_CATALOG_FILES=$(DOCBOOK_CATALOG_FILE) XML_DEBUG_CATALOG=0 \
	${XSLTPROC} \
	--output $@ \
	$(DOCBOOK_XSL_DIR)/template/titlepage.xsl \
	$(DOCBOOK_FO_TITLEPAGE_TEMPLATE_SPEC)

# %.in -> %
%:	%.in
	$(CP) $< sed_tmp.txt
	sed "s|@DOCBOOK_XML_DIR@|$(DOCBOOK_XML_DIR)|g" < sed_tmp.txt > $@
	$(CP) $@ sed_tmp.txt
	sed "s|@DOCBOOK_XSL_DIR@|$(DOCBOOK_XSL_DIR)|g" < sed_tmp.txt > $@
	$(RM) sed_tmp.txt

# xml -> html
%.html:	%.xml $(CATALOG_FILE) $(MENU_FILE)
	env XML_CATALOG_FILES=$(CATALOG_FILE) XML_DEBUG_CATALOG=0 \
	$(XSLTPROC) $(XSLTPROC_FLAGS)  \
	$(DOCBOOK_DEFAULT_PARAMS) $(DOCBOOK_HTML_PARAMS) \
	--output $@ $(DOCBOOK_XSL_DIR)/html/docbook.xsl $<

# xml -> svg
%.svg:	../%.xml menu2svg.xsl
	$(XSLTPROC) $(XSLTPROC_FLAGS) --output $@ menu2svg.xsl $<

# xml -> fo
%.fo:	%.xml $(CATALOG_FILE) $(DOCBOOK_FO_TITLEPAGE_TEMPLATE) $(MENU_FILE) \
	$(PUBDATE_FILE) $(DATE_FILE)
	env XML_CATALOG_FILES=$(CATALOG_FILE) XML_DEBUG_CATALOG=0 \
	$(XSLTPROC) $(XSLTPROC_FLAGS) \
	$(DOCBOOK_DEFAULT_PARAMS) $(DOCBOOK_FOP_PARAMS) \
	--output $@ $(DOCBOOK_FO_STYLESHEET) $<

# fo -> pdf
%.pdf:	%.fo
	$(FOP) -fo $< -pdf $@

# .xml -> .db.xml
%.db.xml:	../Brampuino/%.xml menu2docbook.xsl
	$(XSLTPROC) $(XSLTPROC_FLAGS) --output $@ menu2docbook.xsl $<

$(PUBDATE_FILE):	usersmanual.db.xml global.xml $(MENU_DOCBOOK_FILE) \
			$(PUBDATE_FILE_CREATOR)
	$(PUBDATE_FILE_CREATOR) --pubdate >$@

$(DATE_FILE):	usersmanual.db.xml global.xml $(MENU_DOCBOOK_FILE) \
		$(DATE_FILE_CREATOR)
	$(DATE_FILE_CREATOR) >$@


#
# ebook generation
#

# create epub dir
$(DB2EPUB_DIR):
	mkdir -p $@

# create images dirs
$(DB2EPUB_IMAGE_DIR):
	mkdir -p $@

# create icons dir
$(DB2EPUB_ICONS_DIR):
	mkdir -p $@

# copy local images
copy_local_images:	$(DB2EPUB_DIR)
	cp -r local-images $(DB2EPUB_DIR)

# copy images
copy_images:	$(DB2EPUB_ICONS_DIR) copy_local_images
	cp -r $(DOCBOOK_XSL_DIR)/images/* $(DB2EPUB_ICONS_DIR)

# mime type
mimetype:
	echo "application/epub+zip" > $@

# .xml -> ebook
%.epub:	%.xml $(CATALOG_FILE) $(MENU_FILE) \
	mimetype $(DB2EPUB_IMAGE_DIR) $(DB2EPUB_ICONS_DIR) copy_images
	env XML_CATALOG_FILES=$(CATALOG_FILE) XML_DEBUG_CATALOG=0 \
	$(XSLTPROC) $(XSLTPROC_FLAGS) $(DOCBOOK_EPUB_PARAMS) \
	$(DOCBOOK_EPUB_STYLESHEET) $<
        # zip directoryies
	zip -0Xq $@ mimetype
	zip -Xr9D $@ OEBPS META-INF

# end of file
