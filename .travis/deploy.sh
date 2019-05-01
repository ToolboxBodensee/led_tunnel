#!/bin/bash
mkdir .ssh
echo -e "$SSH_DEPLOY_KEY" > .ssh/id_ed25519
chmod 600 .ssh -R
rsync d1_mini.bin web34@releases:$SSH_DEPLOY_PATH/ --progress
rsync nodemcu.bin web34@releases:$SSH_DEPLOY_PATH/ --progress
rsync nodemcuv2.bin web34@releases:$SSH_DEPLOY_PATH/ --progress
rsync version web34@releases:$SSH_DEPLOY_PATH/ --progress
rm .ssh/id_ed25519 -rf
