
#include <iostream>
#include <vector>
#include <tuple>

double findExpectedReturn(const std::vector<std::tuple<double, double, double>>& instrumentsInfo) {
    
    double Psell, Pbuy, profit, total;
    for (const auto& instrument : instrumentsInfo) {
	std::tie(Psell, Pbuy, profit) = instrument;
        total += (Psell * profit) + (Pbuy * profit);
    }
    
    return total;
}

#define numTime(hhmmss) ((((hhmmss[0] - '0') * 10) + (hhmmss[1] - '0')) * (60 * 60) \
                      + (((hhmmss[3] - '0') * 10) + (hhmmss[4] - '0')) * (60)      \
                      + (((hhmmss[6] - '0') * 10) + (hhmmss[7] - '0')))
            
#define numTimeh(hmmss) ((hmmss[0] - '0') * (60 * 60)                              \
                      + (((hmmss[2] - '0') * 10) + (hmmss[3] - '0')) * (60)      \
                      + (((hmmss[5] - '0') * 10) + (hmmss[6] - '0')))

#define picker(time) ((time[7] == 0) * numTimeh(time)) + ((time[7] != 0) * numTime(time))

int main()
{
	std::string t1 = "08:00:03";
	std::string t2 = "08:00:01";

	std::cout << "time: " << picker(time) - picker(t2) << std::endl;


	std::vector<std::tuple<double, double, double>> info {
		{0.5f, 0.5f, 1.0f},
		{0.3f, 0.3f, 1.0f},
		{1.0f, 0.0f, 1.0f}
	};

	std::cout << findExpectedReturn(info) << std::endl;

	return 0;
}
