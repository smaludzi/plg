#
# Makefile
#
# Copyright (C) 2017 Slawomir Maludzinski
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
CC = gcc
LEX = flex
BISON = bison
CFLAGS = -Wall -Wextra -Wno-unused-parameter -g

OBJECTS = plg.o \
          var.o \
          term.o \
          goal.o \
          clause.o \
          query.o \
          program.o \
          hash.o \
          symtab.o \
          semcheck.o \
          bytecode.o \
          gencode.o \
          unify.o \
          unify_term.o \
          hash.o \
          strtab.o \
          object.o \
          gc.o \
          vm.o
SCAN_PAR = scanner.o parser.o

#TEST_HASH = hash.o test_hash.o
#TEST_UNIFY = test_unify.o unify.o
TEST_GC = object.o gc.o test_gc.o

plg: $(OBJECTS) $(SCAN_PAR)

parser.h parser.c: parser.y
	$(BISON) -v --defines=parser.h --output=parser.c parser.y

test_hash: $(TEST_HASH)
test_unify: $(TEST_UNIFY)
test_gc: $(TEST_GC)

deps:
	$(CC) -MM $(TEST_HASH:.o=.c) $(OBJECTS:.o=.c) > .deps

clean:
	rm -f plg $(OBJECTS) $(SCAN_PAR)

rmcore:
	rm core*

%.c:  %.y
	$(YACC) $< -o $@

.PHONY: deps

include .deps

