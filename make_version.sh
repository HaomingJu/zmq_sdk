

##build linux
rm -rf build
mkdir build
cp build.properties.example.linux-x86 build.properties
cp build.properties.local.linux build.properties.local
rm -rf output_linux_x86/*

cd build/
cmake ..;cmake ..;make copy; make -j4 install

cd ..
rm -rf output/zmq_sdk/*
cp -r output_linux_x86/include/ output/zmq_sdk/
cp -r output_linux_x86/lib/ output/zmq_sdk/
./gradlew upload


##build matrix

rm -rf build
mkdir build
cp build.properties.example.matrix build.properties
cp build.properties.local.matrix build.properties.local
rm -rf output_linux_arm-altera/*
cd build
cmake ..;cmake ..;make copy; make -j4 install
cd ..
rm -rf output/zmq_sdk/*
cp -r output_linux_arm-altera/include/ output/zmq_sdk/
cp -r output_linux_arm-altera/lib/ output/zmq_sdk/
./gradlew upload

##build android

rm -rf build
mkdir build
cp build.properties.example.andriod build.properties
cp build.properties.local.android build.properties.local
rm -rf output_android_arm/*
cd build
cmake ..;cmake ..;make copy; make -j4 install
cd ..
rm -rf output/zmq_sdk/*
cp -r output_android_arm/include/ output/zmq_sdk/
cp -r output_android_arm/lib/ output/zmq_sdk/
./gradlew upload
