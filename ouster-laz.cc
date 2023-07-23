#include <iostream>
#include <rapidcsv.h>
#include <cstdint>

int main(int const argc, char const * const * const argv) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	if (argc != 4) {
		std::cout
			<< "Incorrect number of arguments\n"
			<< "\n"
			<< "usage:\n"
			<< "\t<imu_file> <pcap_file> <json_file>\n"
		;
		return -1;
	}

	char const* const imu_file = argv[1];
	char const* const pcap_file = argv[2];
	char const* const json_file = argv[3];

	rapidcsv::Document const imu = rapidcsv::Document(imu_file);

	// std::vector<uint_fast32_t> timestamp = imu.GetRow<uint_fast32_t>("Unix Time");
	// std::size_t const imu_rows = timestamp.size();

	std::size_t const imu_rows = imu.GetRowCount();

	std::vector<uint_fast64_t> timestamp;
	{
		timestamp.reserve(imu_rows);
		std::vector<uint_fast32_t> unix_time = imu.GetColumn<uint_fast32_t>("Unix Time");
		std::vector<uint_fast32_t> microseconds = imu.GetColumn<uint_fast32_t>("Microseconds");
		for (std::size_t i = 0; i < imu_rows; ++i)
			timestamp.push_back(static_cast<uint_fast64_t>(unix_time[i]) * 1000000 + static_cast<uint_fast64_t>(microseconds[i]));
	}

	for (std::size_t i = 0; i < imu_rows; ++i)
		std::cout << timestamp[i] << "\n";
	std::cout << imu_rows << "\n";

	std::cout << "Hello, world!\n";
	return 0;
}
