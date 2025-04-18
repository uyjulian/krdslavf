#############################################################################
##                                                                         ##
##                    Copyright (C) 2019-2021 Julian Uy                    ##
##                  https://sites.google.com/site/awertyb                  ##
##                                                                         ##
##   This program is free software; you can redistribute it and/or modify  ##
##   it under the terms of the GNU General Public License as published by  ##
##    the Free Software Foundation; either version 2 of the License, or    ##
##                   (at your option) any later version.                   ##
##                                                                         ##
##     This program is distributed in the hope that it will be useful,     ##
##     but WITHOUT ANY WARRANTY; without even the implied warranty of      ##
##      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      ##
##               GNU General Public License for more details.              ##
##                                                                         ##
## You should have received a copy of the GNU General Public License along ##
## with this program; if not, write to the Free Software Foundation, Inc., ##
##       51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.       ##
##                                                                         ##
#############################################################################

SOURCES += main.cpp

LDLIBS += -lshlwapi -luuid -lstrmiids -lole32

PROJECT_BASENAME = krdslavf

RC_DESC ?= LAVFilters connector for TVP(KIRIKIRI) (2/Z)
RC_PRODUCTNAME ?= LAVFilters connector for TVP(KIRIKIRI) (2/Z)
RC_LEGALCOPYRIGHT ?= Copyright (C) 2019-2019 Julian Uy; This product is licensed under the GNU General Public License version 2 or (at your option) any later version.

include external/ncbind/Rules.lib.make
