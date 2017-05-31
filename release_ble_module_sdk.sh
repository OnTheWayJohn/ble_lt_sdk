#record sdk version
SDK_VERSION=1.0

dir_release=ble_module_sdk_release

rm -rf ../$dir_release
mkdir ../$dir_release
cp -rf proj proj_lib  ../$dir_release

mkdir ../$dir_release/vendor
cp -rf vendor/826x_ble_remote ../$dir_release/vendor
cp -rf vendor/826x_module ../$dir_release/vendor
cp -rf vendor/826x_ota_boot ../$dir_release/vendor
cp -rf vendor/8266_host_dongle ../$dir_release/vendor
cp -rf vendor/common ../$dir_release/vendor


cp * .project .cproject ../$dir_release
find ../$dir_release/proj_lib -name *.a |xargs rm -rf
rm -rf ../$dir_release/release_ble_module_sdk.sh
rm -rf ../$dir_release/readme.git.txt
rm -rf ../$dir_release/readme.txt
rm -rf ../$dir_release/ven_release.bat
rm -rf ../$dir_release/build_release.bat

cp proj_lib/liblt_8261_mod.a ../$dir_release/proj_lib
cp proj_lib/liblt_8266_mod.a ../$dir_release/proj_lib
cp proj_lib/liblt_8267_mod.a ../$dir_release/proj_lib
cp proj_lib/liblt_8266_32m.a ../$dir_release/proj_lib
cp proj_lib/liblt_8261.a ../$dir_release/proj_lib
cp proj_lib/liblt_8266.a ../$dir_release/proj_lib
cp proj_lib/liblt_8267_32m.a ../$dir_release/proj_lib
cp proj_lib/liblt_8267_48m.a ../$dir_release/proj_lib
cp proj_lib/liblt_8267.a ../$dir_release/proj_lib

