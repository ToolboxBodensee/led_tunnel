#!/bin/bash
echo "releases.tbbs.me ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBC5NslYj+wtriBuKIxJfDRm9E2hnlstWR8durQ6pKvVvP7wjcoXIyAxe41GvJ9SoEhSjF7oVlzlw+zAkzl5WOZ4=" >> $HOME/.ssh/known_hosts
echo -e "$SSH_DEPLOY_KEY" > $HOME/.ssh/id_ed25519
chmod 600 .ssh -R
rsync d1_mini.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rsync nodemcu.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rsync nodemcuv2.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rsync version web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rm -rf $HOME/.ssh/id_ed25519
