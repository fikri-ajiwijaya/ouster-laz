#include <iostream>
#include <rapidcsv.h>
#include <cstdint>
#include <proj.h>

#include <iomanip>

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

	// for (std::size_t i = 0; i < imu_rows; ++i)
	// 	std::cout << timestamp[i] << "\n";
	// std::cout << imu_rows << "\n";

	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> z;
	{
		x.reserve(imu_rows);
		y.reserve(imu_rows);
		z.reserve(imu_rows);

		PJ *P;
		{
			P = proj_create_crs_to_crs(nullptr, "EPSG:4979", "EPSG:7415", nullptr);
			if (!P) {
				std::cerr << "Failed to create transformation object.\n";
				return -1;
			}
			PJ *norm = proj_normalize_for_visualization(nullptr, P);
			if (!norm) {
				std::cerr << "Failed to normalize transformation object.\n";
				return -1;
			}
			proj_destroy(P);
			P = norm;
		}

		std::vector<double> longitude = imu.GetColumn<double>("Longitude");
		std::vector<double> latitude = imu.GetColumn<double>("Latitude");
		std::vector<double> height = imu.GetColumn<double>("Height");

		for (std::size_t i = 0; i < imu_rows; ++i) {
			PJ_COORD a = proj_coord(longitude[i], latitude[i], height[i], 0);
			PJ_COORD b = proj_trans(P, PJ_FWD, a);
			x.push_back(b.xyzt.x);
			y.push_back(b.xyzt.y);
			z.push_back(b.xyzt.z);
		}

		std::cout << std::fixed << std::showpoint << std::setprecision(8);
		for (std::size_t i = 0; i < imu_rows; ++i)
			std::cout << "[" << longitude[i] << ", " << latitude[i] << ", " << height[i] << "]    [" << x[i] << ", " << y[i] << ", " << z[i] << "]\n";
		std::cout << imu_rows << "\n";

		proj_destroy(P);
	}

	std::cout << "Hello, world!\n";
	return 0;
}
