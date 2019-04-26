git config --local user.name "traviscibot"
git config --local user.email "traviscibot@travisci.org"
git checkout master
git tag -d latest
git tag -a latest
git push origin latest -f
