#!/bin/bash

# Change the line below because, when run from cron, we're not
# in the right directory.

cd ~minerva/update/tv

./gettv.sh
./process_searches.sh

