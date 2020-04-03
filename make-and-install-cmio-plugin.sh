cd build
make mac-cmio
retVal=$?
if [ $retVal -ne 0 ]; then
    echo "Error"
    exit $retVal
fi
cp ./rundir/RelWithDebInfo/obs-plugins/mac-cmio.so ../mac-vcam-plugin-pkg/
echo "Enter sudo password to copy plugin to /Applications/OBS.app/Contents/Plugins/"
sudo cp ./rundir/RelWithDebInfo/obs-plugins/mac-cmio.so /Applications/OBS.app/Contents/Plugins/
cd -
