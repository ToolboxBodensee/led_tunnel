#!/bin/bash
echo "releases.tbbs.me ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBC5NslYj+wtriBuKIxJfDRm9E2hnlstWR8durQ6pKvVvP7wjcoXIyAxe41GvJ9SoEhSjF7oVlzlw+zAkzl5WOZ4=" >> $HOME/.ssh/known_hosts
echo -e "$SSH_DEPLOY_KEY" > $HOME/.ssh/id_ed25519
chmod 600 $HOME/.ssh/id_ed25519
ls -la $HOME
ls -la 
rsync version web34@releases.tbbs.me:$SSH_DEPLOY_PATH/version --progress
rsync .pioenvs/nodemcu/firmware.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/nodemcu.bin --progress
rsync .pioenvs/d1_mini/firmware.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/d1_mini.bin --progress
rsync .pioenvs/nodemcuv2/firmware.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/nodemcuv2.bin --progress
rm -rf $HOME/.ssh/id_ed25519
