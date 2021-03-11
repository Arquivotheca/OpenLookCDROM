/**/#!/bin/sh
/**/#
/**/#	@(#)mdepend.sh	1.3	5/11/88 19:44:36
/**/#
/**/#	Do the equivalent of the 'makedepend' program, but do it right.
/**/#
/**/#	Usage:
/**/#
/**/#	makedepend [cpp-flags] [-w width] [-s magic-string] [-f makefile]
/**/#	  [-o object-suffix]
/**/#
/**/#	Notes:
/**/#
/**/#	The C compiler used can be overridden with the environment
/**/#	variable "CC".
/**/#
/**/#	The "-v" switch of the "makedepend" program is not supported.
/**/#
/**/#
/**/#	This script should
/**/#	work on both USG and BSD systems.  However, when System V.4 comes out,
/**/#	USG users will probably have to change "silent" to "-s" instead of
/**/#	"-" (at least, that's what the documentation implies).
/**/#
CC=PREPROC

silent='-'

TMP=/tmp/mdep$$
CPPCMD=${TMP}a
DEPENDLINES=${TMP}b
TMPMAKEFILE=${TMP}c
MAGICLINE=${TMP}d
ARGS=${TMP}e

trap "rm -f ${TMP}*; exit 1" 1 2 15
trap "rm -f ${TMP}*; exit 0" 1 2 13

echo " \c" > $CPPCMD
if [ `wc -c < $CPPCMD` -eq 1 ]
then
    c="\c"
    n=
else
    c=
    n="-n"
fi

echo $n "$c" >$ARGS

files=
makefile=
magic_string='# DO NOT DELETE'
objsuffix='.o'
width=78
endmarker=""
verbose=n

while [ $# != 0 ]
do
    if [ "$endmarker"x != x -a "$endmarker" = "$1" ]; then
	endmarker=""
    else
	case "$1" in
	    -D*|-I*)
		echo $n " '$1'$c" >> $ARGS
		;;

	    -g|-o)
		;;

	    *)
		if [ "$endmarker"x = x ]; then
		    case "$1" in 	
			-w)
			    width="$2"
			    shift
			    ;;
			-s)
			    magic_string="$2"
			    shift
			    ;;
			-f)
			    makefile="$2"
			    shift
			    ;;
			-o)
			    objsuffix="$2"
			    shift
			    ;;
			
			--*)
			    echo "$1" | sed 's/^\-\-//' >${TMP}end
			    endmarker="`cat ${TMP}end`"
			    rm -f ${TMP}end
			    if [ "$endmarker"x = x ]; then
				endmarker="--"
			    fi
			    ;;
			-v)
			    verbose="y"
			    ;;

			-cc)
			    CC="$2"
			    shift
			    ;;

			-*)
			    echo "Unknown option '$1' ignored" 1>&2
			    ;;
			*)
			    files="$files $1"
			    ;;
		    esac
		fi
		;;
	esac
    fi
    shift
done
echo ' $*' >> $ARGS

echo "exec $CC `cat $ARGS`" > $CPPCMD
chmod +x $CPPCMD
rm $ARGS

case "$makefile" in
    '')
	if [ -r makefile ]
	then
	    makefile=makefile
	elif [ -r Makefile ]
	then
	    makefile=Makefile
	else
	    echo 'no makefile or Makefile found' 1>&2
	    exit 1
	fi
	;;
    -)
	makefile=$TMPMAKEFILE
	;;
esac

if [ "$verbose"x = "y"x ]; then 
    cat $CPPCMD
fi

echo '' > $DEPENDLINES
for i in $files
do
    $CPPCMD $i \
      | sed -n "/^#/s;^;$i ;p"
done \
  | sed -e 's|/[^/.][^/]*/\.\.||g' -e 's|/\.[^.][^/]*/\.\.||g' \
    -e 's|"||g' -e 's| \./| |' \
  | awk '{
	if ($1 != $4  &&  $2 != "#ident")
	    {
	    ofile = substr ($1, 1, length ($1) - 2) "'"$objsuffix"'"
	    print ofile, $4
	    }
	}' \
  | sort -u \
  | awk '
	    {
	    newrec = rec " " $2
	    if ($1 != old1)
		{
		old1 = $1
		if (rec != "")
		    print rec
		rec = $1 ": " $2
		}
	    else if (length (newrec) > '"$width"')
		{
		print rec
		rec = $1 ": " $2
		}
	    else
		rec = newrec
	    }
	END \
	    {
	    if (rec != "")
		print rec
	    }' \
  | egrep -v '^[^:]*:[ 	]*$' >> $DEPENDLINES

trap "" 1 2 13 15	# Now we are committed
case "$makefile" in
    $TMPMAKEFILE)
	;;
    *)
	rm -f Makefile.bak
	cp $makefile $makefile.bak
	echo "Appending dependencies to $makefile"
	;;
esac

/**/#
/**/# Append the magic string and a blank line so that /^$magic_string/+1,\$d
/**/# can be used to delete everything from after the magic string to the end
/**/# of the file.  Then, append a blank line again and then the dependencies.
/**/#
cat >> $makefile <<- END_OF_APPEND

	$magic_string

END_OF_APPEND
ed $silent $makefile <<- END_OF_ED_SCRIPT
/^$magic_string/+1,\$d
w
q
END_OF_ED_SCRIPT
echo '' >>$makefile
cat $DEPENDLINES >>$makefile

case "$makefile" in
    $TMPMAKEFILE)
	cat $TMPMAKEFILE
	;;

esac

rm -f ${TMP}*
exit 0
