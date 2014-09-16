#BHEADER**********************************************************************
#
# Copyright (c) 2013, Lawrence Livermore National Security, LLC. 
# Produced at the Lawrence Livermore National Laboratory. Written by 
# Jacob Schroder schroder2@llnl.gov, Rob Falgout falgout2@llnl.gov,
# Tzanio Kolev kolev1@llnl.gov, Ulrike Yang yang11@llnl.gov, 
# Veselin Dobrev dobrev1@llnl.gov, et al. 
# LLNL-CODE-660355. All rights reserved.
# 
# This file is part of XBraid. Email schroder2@llnl.gov on how to download. 
# 
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License (as published by the Free Software
# Foundation) version 2.1 dated February 1999.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the terms and conditions of the GNU General Public
# License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License along
# with this program; if not, write to the Free Software Foundation, Inc., 59
# Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
#EHEADER**********************************************************************

##################################################################
# Import machine specific compilers, options, flags, etc.. 
##################################################################

include makefile.inc


##################################################################
# Targets
##################################################################

BRAID_HEADERS = _braid.h braid.h util.h braid_test.h braid_status.h braid_defs.h

BRAID_FILES = util.c braid.c _braid.c braid_test.c braid_status.c

BRAID_OBJ = $(BRAID_FILES:.c=.o)

.PHONY: examples
.SUFFIXES:
.SUFFIXES: .c .o

# Rule for compiling .c files
%.o: %.c $(BRAID_HEADERS)
	$(MPICC) $(CFLAGS) -c $< -o $@

libbraid.a: $(BRAID_HEADERS) $(BRAID_OBJ)
	@echo "Building" $@ "..."
	ar cruv libbraid.a $(BRAID_OBJ)
	ranlib libbraid.a

all: libbraid.a examples

examples: libbraid.a
	cd examples; $(MAKE)

clean:
	rm -f *.o libbraid.a
	cd examples; $(MAKE) clean
