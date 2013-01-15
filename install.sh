#!/bin/bash
MINBIN=/usr/local/minerva/bin
MINSBIN=/usr/local/minerva/sbin
WEBBASE=/var/www/sites/homecontrol/minerva

echo
echo Minerva - Installation
echo
echo IMPORTANT - Unless you are familar with the system, it is recommended
echo you install everything.
echo
echo

create_users()
{
   groupadd minerva
   useradd -g minerva minerva
   usermod -G minerva -a www-data
}

do_main_install()
{
   mkdir -p /usr/local/minerva
   cp -a base/*   /usr/local/minerva

   mkdir -p /var/log/minerva/bearskin
   mkdir -p /var/log/minerva/cache
   mkdir -p /var/log/minerva/cosmic
   mkdir -p /var/log/minerva/minx
   mkdir -p /var/log/minerva/rss
   mkdir -p /var/log/minerva/infostream
   mkdir -p /var/log/minerva/infostream/action
   mkdir -p /var/log/minerva/infostream/event
   mkdir -p /var/log/minerva/infostream/status

   mkdir -p ~minerva/update
   cp -a update/*  ~minerva/update

   cp -a conf ~minerva

   $MINSBIN/msginstall
}

do_bearskin_initialize()
{
   if [ -f /usr/local/bin/heyu ]; then
      /usr/local/bin/heyu engine
   fi

   $MINBIN/cdplayer    default init
   $MINBIN/cosmic      default init
   $MINBIN/ctimer      default init
   $MINBIN/manifest    default init   
   $MINBIN/monexec     default init   
   $MINBIN/mixer       default init
   $MINBIN/mp3player   default init
   $MINBIN/todo        default init
   $MINBIN/x10control  default init   
   $MINBIN/videostream default init   
   $MINBIN/fmradio		default init   

   # separate files (old?)
   touch /var/log/minerva/bearskin/msgconduit  
   touch /var/log/minerva/bearskin/addminervaapplet  
   touch /var/log/minerva/bearskin/stopmedia    
   touch /var/log/minerva/bearskin/alarm
   touch /var/log/minerva/bearskin/fmradio
   touch /var/log/minerva/bearskin/lstatus
   touch /var/log/minerva/bearskin/rlyexec
   touch /var/log/minerva/bearskin/msgrcv
   touch /var/log/minerva/bearskin/status
   touch /var/log/minerva/bearskin/mstatus
}

do_web_install()
{
   mkdir -p $WEBBASE
   cp -a web/* $WEBBASE
   ln -s /usr/local/minerva/media/images/tvguide/ $WEBBASE/warp/conf/tvguide/images

   apache2ctl restart
}

do_tv_install()
{
   pushd conf/tv >/dev/null

   ./make_web.sh
   ./prepare_web.sh

   cp -a stations ~minerva/update/tv

   popd >/dev/null
}

do_daily_update()
{
   pushd conf/tv  >/dev/null
   ~minerva/update/tv/daily_process.sh
   ~minerva/update/weather
   popd
}

do_minauth()
{
   $MINBIN/minuser auth $1 cdplayer set $2
   $MINBIN/minuser auth $1 bluetooth set $2
   $MINBIN/minuser auth $1 system set $2
   $MINBIN/minuser auth $1 mixer set $2
   $MINBIN/minuser auth $1 radio set $2
   $MINBIN/minuser auth $1 mp3jukebox set $2
   $MINBIN/minuser auth $1 fmradio set $2
   $MINBIN/minuser auth $1 photos set $2
   $MINBIN/minuser auth $1 videostream set $2
   $MINBIN/minuser auth $1 tv set $2
   $MINBIN/minuser auth $1 vlc set $2
   $MINBIN/minuser auth $1 x10 set $2
   $MINBIN/minuser auth $1 sms set $2
}

do_chmod()
{

   chown -R minerva $WEBBASE /var/log/minerva ~minerva/update
   chgrp -R minerva $WEBBASE /var/log/minerva ~minerva/update

   # Ideally, I'd like to only assign +rw to ug, but cosmic/heyu runs
   # the scripts as a user without the group assigned. (but only
   # sometimes :( Solutions are requested ;)
   chmod -R ugo+rw  $WEBBASE /var/log/minerva ~minerva/update
}

do_heyu()
{
   chgrp minerva /dev/ttyS0
}


echo;echo;echo;
echo Press ENTER to copy the base system to /usr/local/minerva
echo and create the /var/log/minerva stubs.
echo Or type any other text to skip this step.
read -p "> "

if [ "$REPLY" == "" ]; then
   do_main_install;
   do_bearskin_initialize;
fi


echo Press ENTER to create a user and group called 'minerva'
echo or any other text to skip this step.
echo Note: This will fail if you are not root!
read -p "> "

if [ "$REPLY" == "" ]; then
  create_users;
fi


echo;echo;echo;
echo Press ENTER to create an 'everyone' Minerva user.
echo or any other text to skip this step.
echo The 'everyone' user creates full TV search pages, currently,
echo and will ultimately support 'guest' access.
read -p "> "
if [ "$REPLY" == "" ];then
   /usr/local/minerva/bin/addminervauser everyone
   /usr/local/minerva/bin/addminervauser public
fi


echo;echo;echo;
echo Please enter the name of each member of the household that will
echo use Minerva. This is needed for the TV guide, amongst other things.
echo You can add new users with the 'addminervauser' command.
echo Press ENTER on its own to terminate.
REPLY=q
while [ "$REPLY" != "" ]
do
read
if [ "$REPLY" != "" ];then
   /usr/local/minerva/bin/addminervauser $REPLY
fi
done


echo;echo;echo;
echo Please enter the name of each member of the household that will
echo have READ and WRITE access to the protocols.
echo You can amend specifics later with the 'minuser' command
echo Press ENTER on its own to terminate.
REPLY=q
while [ "$REPLY" != "" ]
do
read
if [ "$REPLY" != "" ];then
   do_minauth $REPLY rw
fi
done



echo;echo;echo;
echo Please enter the name of each member of the household that will
echo have READ ONLY access to the protocols.
echo You can amend specifics later with the 'minuser' command
echo Press ENTER on its own to terminate.
REPLY=q
while [ "$REPLY" != "" ]
do
read
if [ "$REPLY" != "" ];then
   do_minauth $REPLY ro
fi
done

echo;echo;echo;
echo Press ENTER to copy the web components system to $WEBBASE
echo or any other text to skip this step.
read -p "> "
if [ "$REPLY" == "" ];then
   do_web_install;
fi


echo;echo;echo;
echo Press ENTER to prepare the TV guide web pages
echo or any other text to skip this step.
read -p "> "
if [ "$REPLY" == "" ];then
   do_tv_install;
fi


echo;echo;echo;
echo Ready to initialize the radio stations...
echo If you would like to search for stations in a specific area
echo then please enter that location in now. e.g. London
echo Otherwise, press enter.
echo \(I admit, the search isn't great for smaller areas because the 
echo area names given are very specific. If you'd like to help improve the
echo XML then please email me updates\)
read -p "> "

if [ "$REPLY" == "" ]; then
  REPLY=London
fi

xsltproc --stringparam SEARCH_PLACE $REPLY setup/stationfind.xsl setup/fmstations.xml >/usr/local/minerva/conf/fmstations.conf 


echo;echo;echo;
echo Press ENTER to run the daily downloads
echo or any other text to skip this step.
echo \(Requires internet connection\)
read -p "> "
if [ "$REPLY" == "" ]; then
   do_daily_update;
fi

do_chmod;

echo Press ENTER to change the /dev/ttyS0 device to the Minerva group
echo "(as necessary for heyu control via the web)"
read -p "> "

if [ "$REPLY" == "" ]; then
  do_heyu;
fi


echo;echo;echo;
echo Installation complete!
echo
cat POST_INSTALL

