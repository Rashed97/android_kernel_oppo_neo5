# Setups up the kernel build.
# Mac OS-X Version
ARCH=arm CROSS_COMPILE=/Volumes/cm/prebuilts/gcc/darwin-x86/arm/arm-eabi-4.8/bin/arm-eabi- make -j8

echo "checking for compiled kernel..."
if [ -f arch/arm/boot/zImage ]
then

echo "DONE :D "

fi
