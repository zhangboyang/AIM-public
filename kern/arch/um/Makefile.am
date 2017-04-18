# Copyright (C) 2016 David Gao <davidgao1001@gmail.com>
#
# This file is part of AIM.
#
# AIM is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# AIM is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

include $(top_srcdir)/env.am

dist_noinst_DATA = vmaim.lds.S

noinst_LTLIBRARIES = \
	libarch.la libentry.la libum.la

libarch_la_SOURCES = \
    console.c arch_mmu.c
libarch_la_LIBADD = libum.la

libum_la_SOURCES = \
    um-entry.c um-mmu.c um-debug.c

libum_la_CPPFLAGS = $(patsubst -I%, -iquote%, $(filter-out -nostdinc, $(AM_CPPFLAGS)))


libentry_la_SOURCES = \
    entry.c

