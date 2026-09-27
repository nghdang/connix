# Makefile for Doxygen docstring documentation

DOXYGEN  ?= doxygen
DOXYFILE ?= Doxyfile
BUILDDIR = _build

.PHONY: all html clean help

all: html

help:
	@echo "Usage: make [html|clean|help]"

html:
	@$(DOXYGEN) $(DOXYFILE)

clean:
	@rm -rf $(BUILDDIR)
