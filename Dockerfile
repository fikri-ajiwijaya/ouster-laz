from ubuntu
run apt-get update && apt-get -y dist-upgrade
run apt-get -y install git
run apt-get -y install build-essential cmake libjsoncpp-dev libeigen3-dev libcurl4-openssl-dev libtins-dev libpcap-dev libglfw3-dev libglew-dev libspdlog-dev
run apt-get -y install libproj-dev
run sed -i 's`; network = on`network = on`' /usr/share/proj/proj.ini
