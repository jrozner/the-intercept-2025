if [ ! -d build ]; then mkdir build; fi
	
pushd build > /dev/null
cmake -DPICO_SDK_PATH=$PICO_SDK_PATH -DPICO_PLATFORM=rp2350 -DPICO_BOARD=pico2 ..
make

if [ "$1" != "" ]; then
    echo "Supressing flash due to cli arg. Butts."
else
    pico_path=`which picotool`
    if [ "$pico_path" == "" ]; then
        pico_path="./_deps/picotool/picotool" 
    fi
    sudo $pico_path load firmware.elf
fi

popd > /dev/null
