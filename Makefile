#######################################################################
#
#  pwgen - The random string generator
#  Copyright (C) 2001  Marcus Jansson <mjansson256@yahoo.se>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#######################################################################

PROGRAM = pwgen
ifndef $(INSTALLDIR)
	INSTALLDIR = /usr/local/sbin
endif
SOURCES = main.c

CC = gcc
CFLAGS =
LDFLAGS =

all:
	$(CC) $(CFLAGS) $(SOURCES) -o$(PROGRAM)

install:
	cp $(PROGRAM) $(INSTALLDIR)

clean:
	rm -rf *.o $(PROGRAM)
