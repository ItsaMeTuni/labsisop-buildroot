#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

# set default password for root to be "root"
# dropbear requires a password to be set for the user
sed -i -e 's/root:/root:NFO7WjQHjLDNs/g' $BASE_DIR/target/etc/shadow