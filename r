cd build
make
retVal=$?
if [ $retVal -ne 0 ]; then
    echo "Error"
    exit $retVal
fi
clear
cd rundir/RelWithDebInfo/bin
./obs
cd ../../../..
