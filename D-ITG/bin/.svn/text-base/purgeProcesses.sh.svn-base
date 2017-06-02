#!/bin/bash

# This scripts kills all the processes whose name matches $PMATCH and which are in execution
# since more than 10 minutes...

MINUTES=10
PMATCH='ITGSend'

/usr/bin/find /proc/ \
        -maxdepth 3 \
        -name 'cmdline' \
        -mmin +${MINUTES} \
        -exec /bin/grep -l ${PMATCH} {} \; | \
                /usr/bin/cut -d'/' -f3 | \
                /usr/bin/xargs -n1 -i^ /bin/bash -c '/bin/kill ^ 2>&1 > /dev/null'
