# -*- Makefile -*-

lib_TARGETS = gui

# Tu peux rajouter des fichiers source ici
gui-dists = prologin.py

# Evite de toucher a ce qui suit
gui-dists += api.py interface.hh
gui-srcs = interface.cc

gui-cxxflags = -fPIC $(shell python3-config --embed --includes)
gui-ldflags = -s $(shell python3-config --embed --ldflags)

STECHEC_LANG=python
include ../includes/rules.mk
