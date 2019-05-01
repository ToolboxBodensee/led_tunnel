#!/bin/bash
mkdir $HOME/.ssh
echo -e "$SSH_DEPLOY_KEY" > $HOME/.ssh/id_ed25519
chmod 600 .ssh -R
rsync d1_mini.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rsync nodemcu.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rsync nodemcuv2.bin web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rsync version web34@releases.tbbs.me:$SSH_DEPLOY_PATH/ --progress
rm -rf $HOME/.ssh/id_ed25519
