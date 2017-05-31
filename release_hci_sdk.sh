#record sdk version
SDK_VERSION=1.0
echo "Telink BLE SDK_VERSION = $SDK_VERSION" > ./sdk_version.txt

dir_release=ble_hci_sdk

rm -rf ../$dir_release
mkdir ../$dir_release
cp -rf proj proj_lib vendor ../$dir_release
cp * .project  ../$dir_release
cp hci.cproject ../$dir_release/.cproject
rm -rf ../$dir_release/release_sdk.sh
rm -rf ../$dir_release/readme.git.txt
rm -rf ../$dir_release/readme.txt
rm -rf ../$dir_release/ven_release.bat
rm -rf ../$dir_release/build_release.bat

rm -rf ../$dir_release/vendor/826x_ota_boot
rm -rf ../$dir_release/vendor/8267_ble_rc_demo
rm -rf ../$dir_release/vendor/8267_ble_remote
rm -rf ../$dir_release/vendor/8267_mcu_rc
rm -rf ../$dir_release/vendor/flyco_8266_module





