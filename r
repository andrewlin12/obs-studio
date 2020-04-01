cd build
make
retVal=$?
if [ $retVal -ne 0 ]; then
    echo "Error"
    exit $retVal
fi
clear
cd rundir/RelWithDebInfo/bin
if [ "$MYUSERNAME" = "walt" ]; then
    install_name_tool -add_rpath /Users/walt/Qt5.5.1/5.5/clang_64/lib obs
fi
./obs
cd ../../../..
