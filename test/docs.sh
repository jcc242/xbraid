#!/bin/sh
#BHEADER**********************************************************************
# Copyright (c) 2013,  Lawrence Livermore National Security, LLC.
# Produced at the Lawrence Livermore National Laboratory.
# This file is part of XBraid.  See file COPYRIGHT for details.
#                                                                          
# XBraid is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License (as published by the Free
# Software Foundation) version 2.1 dated February 1999.                    
#
#EHEADER**********************************************************************

# scriptname holds the script name, with the .sh removed
scriptname=`basename $0 .sh`

# Echo usage information
case $1 in
   -h|-help)
      cat <<EOF

   $0 [-h|-help] 

   where: -h|-help   prints this usage information and exits

   This script performs a git checkout of the HEAD and then does a test compile
   of the code for a tux box.  The output is written to $scriptname.out, 
   $scriptname.err and $scriptname.dir. This test 
   passes if $scriptname.err is empty.

   Example usage: ./test.sh $0 

EOF
      exit
      ;;
esac

# Setup
share_dir="/usr/casc/hypre/braid/share/braid_manuals/"
example_dir="../examples"
test_dir=`pwd`
docs_dir=`cd ../docs; pwd`
output_dir=`pwd`/$scriptname.dir
rm -fr $output_dir
mkdir -p $output_dir

# Run the following regression tests 
TESTS=( "cd $docs_dir; make clean; make developer_manual " \
        'PDFsize=$(du -k $docs_dir/developer_manual.pdf | cut -f 1); if [ $PDFsize -le 700 ] ; then echo docs/developermanual.pdf is too small ; else echo docs/developermanual.pdf is appropriately sized ; fi  ' \
        "cd $docs_dir; cp developer_manual.pdf $share_dir" \
        "cd $docs_dir; make clean; make user_manual "
        'PDFsize=$(du -k $docs_dir/user_manual.pdf | cut -f 1); if [ $PDFsize -le 500 ] ; then echo docs/usermanual.pdf is too small ; else echo docs/usermanual.pdf is appropriately sized ; fi  ' \
        "cd $docs_dir; cp user_manual.pdf $share_dir" )

# The below commands will then dump each of the tests to the output files 
#   $output_dir/unfiltered.std.out.0, 
#   $output_dir/std.out.0, 
#   $output_dir/std.err.0,
#    
#   $output_dir/unfiltered.std.out.1,
#   $output_dir/std.out.1, 
#   $output_dir/std.err.1,
#   ...
#
# The unfiltered output is the direct output of the script, whereas std.out.*
# is filtered by a grep for the lines that are to be checked.  
#
lines_to_check=".*Error.*|.*error.*|.*pdf is appropriately sized|.*pdf is too small"
#
# Then, each std.out.num is compared against stored correct output in 
# $scriptname.saved.num, which is generated by splitting $scriptname.saved
#
TestDelimiter='# Begin Test'
csplit -n 1 --silent --prefix $output_dir/$scriptname.saved. $scriptname.saved "%$TestDelimiter%" "/$TestDelimiter.*/" {*}
#
# The result of that diff is appended to std.err.num. 

# Run regression tests
counter=0
for test in "${TESTS[@]}"
do
   echo "Running Test $counter"
   eval "$test" 1>> $output_dir/unfiltered.std.out.$counter  2>> $output_dir/std.err.$counter
   cd $output_dir
   egrep -o "$lines_to_check" unfiltered.std.out.$counter > std.out.$counter
   diff -U3 -B -bI"$TestDelimiter" $scriptname.saved.$counter std.out.$counter >> std.err.$counter
   cd $test_dir
   counter=$(( $counter + 1 ))
done 

# Echo to stderr all nonempty error files in $output_dir.  test.sh
# collects these file names and puts them in the error report
for errfile in $( find $output_dir ! -size 0 -name "*.err.*" )
do
   echo $errfile >&2
done

