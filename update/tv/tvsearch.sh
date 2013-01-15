#!/bin/bash
# This takes one parameter. If a file exists with this name
# then it that file contains a comma separate list of search terms.
# if no file exists, the parameter itself is used as the search term(s).

USER=$2
USERPATH=/usr/local/minerva/etc/users

HTML=$USERPATH/$USER/tvresults.inc

if [ -f $1 ]; then
  SEARCH=`cat $1`
else
  SEARCH=$1
fi

echo "<?php" > $HTML
echo "class User$USER extends UserProgrammes {" >>$HTML
echo "   function User$USER() {" >>$HTML

echo "      \$this->_day = array();" >>$HTML

for day in 0 1 2; do

echo "     \$curr = \$this->_day[$day] = new DaysProgramming();" >>$HTML
for i in `ls cache/$day/*.xml`; do
	xsltproc --param SEARCH_TERM "'$SEARCH'" tvphp.xsl $i >>$HTML
done

done

echo "   } " >>$HTML

echo "}" >>$HTML
echo "?>" >>$HTML

