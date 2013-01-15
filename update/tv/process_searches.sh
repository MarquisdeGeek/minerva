#!/bin/bash
# Works through each users list of requests in the 'searches' directory then:
# a. sends an email to them (using the results.htm file)
# b. copies the file to a local web server

MINBASE=/usr/local/minerva
USERDIR=$MINBASE/etc/users

pushd ~minerva/update/tv >/dev/null

for user in `ls $USERDIR`; do
 echo "Processing list for $user"

 if [ -f $USERDIR/$user/tvsearch/list ]; then
   ./tvsearch.sh $USERDIR/$user/tvsearch/list $user
 fi
 
 if [ -f $USERDIR/$user/tvsearch/email ]; then
    $MINBASE/bin/tvreport default $user | html2text | $MINBASE/bin/msgxmit email `head -n 1 $USERDIR/$user/tvsearch/email` 
 fi

 if [ -f $USERDIR/$user/tvsearch/sms ]; then
    $MINBASE/bin/tvreport default $user short | html2text | $MINBASE/bin/msgxmit sms `head -n 1 $USERDIR/$user/tvsearch/sms` 
 fi


done

popd >/dev/null

