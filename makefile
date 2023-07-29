CXXFLAGS := -std=c++17
CPPFLAGS := -I/usr/include/eigen3 -Iinclude
LDFLAGS := -Llib
LDLIBS := -lproj -louster_client -ljsoncpp -lspdlog

ouster := ouster_example/build/ouster_client/libouster_client.a

ouster-laz: ouster-laz.cc | $(ouster)

$(ouster):
	cmake \
		ouster_example \
		-B ouster_example/build \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_PCAP=OFF \
		-DBUILD_OSF=OFF \
		-DBUILD_VIZ=OFF \
		-DBUILD_TESTING=OFF \
		-DBUILD_EXAMPLES=OFF
	cmake --build ouster_example/build

.PHONY: clean
clean:
	rm -f ouster-laz

.PHONY: reset
reset:
	git submodule foreach --recursive git clean -ffdx

include makefile.compile_commands
