#!/bin/bash

echo "installing OBS Mac VCam plugin..."

rm -rf /tmp/mac-vcam-plugin
mkdir /tmp/mac-vcam-plugin
cd /tmp/mac-vcam-plugin
curl -L -o SampleVCam.plugin.zip https://github.com/andrewlin12/obs-studio/raw/mac-cmio/mac-vcam-plugin-pkg/SampleVCam.plugin.zip
curl -L -o mac-cmio.so  https://github.com/andrewlin12/obs-studio/raw/mac-cmio/mac-vcam-plugin-pkg/mac-cmio.so
unzip SampleVCam.plugin.zip
echo "copying plugin to /Library/CoreMediaIO/Plug-Ins/DAL/"
cp -r SampleVCam.plugin /Library/CoreMediaIO/Plug-Ins/DAL/
chmod 755 mac-cmio.so
echo "copying plugin to /Applications/OBS.app/Contents/Plugins/"
cp mac-cmio.so /Applications/OBS.app/Contents/Plugins/
cd -
echo "all done"

exit 0
