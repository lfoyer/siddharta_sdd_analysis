#include <map>
#include <string>

std::set<std::string> elemWithTail = {"Ti_ka", "Fe_ka", "Cu_ka", "Sr_ka"};

// Be warned- this is not very optimal code....
std::set<std::string> calibrationMainElements = {"Ti_ka", "Fe_ka", "Cu_ka", "Sr_ka"};
std::vector<std::string> mainElements = {"Ti_ka", "Fe_ka", "Cu_ka", "Sr_ka"};


std::vector<std::string> calibrationElements = {
	"Ti_ka",
	"Ti_kb",
	"Cr_ka",
	"Cr_kb",
	"Fe_ka",
	"Fe_kb",
	"Cu_ka",
	"Cu_kb",
	"Sr_ka",
	"Sr_kb"};

// Element names for labels; order of strings here determines order of fit parameters/labels
std::vector<std::string> elements = {
	"Ti_ka",
	"Ti_kb",
	"Cr_ka",
	"Mn_ka",
	"Fe_ka",
	"Fe_kb",
	"Ni_ka",
	"Cu_ka",
	"Zn_ka",
	"Cu_kb",
	"Zn_kb",
	"Pb_la",
	"Bi_la",
	"Au_lb",
	"Br_ka",
	"Pb_lb",
	//"Bi_lb",
	"Sr_ka",
	"Sr_kb",
	"Sr_kb2"};

// Initial amplitude of single SDD spectrum
std::map<std::string, Double_t> amp = {
	{"Ti_ka", 12000},
	{"Ti_kb", 1900},
	{"Cr_ka", 600},
	{"Cr_kb", 0},
	{"Mn_ka", 400},
	{"Fe_ka", 26000},
	{"Fe_kb", 4000},
	{"Ni_ka", 0},
	{"Cu_ka", 65000},
	{"Zn_ka", 0},
	{"Cu_kb", 10000},
	{"Zn_kb", 0},
	{"Pb_la", 0},
	{"Bi_la", 0},
	{"Au_lb", 0},
	{"Br_ka", 0},
	{"Pb_lb", 0},
	{"Bi_lb", 0},
	{"Sr_ka", 45000},
	{"Sr_kb", 6500},
	{"Sr_kb2", 0}};

// Initial sigma of single SDD spectrum
std::map<std::string, Double_t> sig = {
	{"Ti_ka", 21.5},
	{"Ti_kb", 24.6},
	{"Cr_ka", 25.6},
	{"Cr_kb", 0},
	{"Mn_ka", 18.8},
	{"Fe_ka", 22.4},
	{"Fe_kb", 29.0},
	{"Ni_ka", 0},
	{"Cu_ka", 23.2},
	{"Zn_ka", 0},
	{"Cu_kb", 24.4},
	{"Zn_kb", 0},
	{"Pb_la", 0},
	{"Br_ka", 0},
	{"Bi_la", 0},
	{"Au_lb", 0},
	{"Br_ka", 0},
	{"Pb_lb", 0},
	{"Bi_lb", 0},
	{"Sr_ka", 28},
	{"Sr_kb", 28},
	{"Sr_kb2", 0}};

// Expected eV values from xray table (ie to match with ADC mean)
std::map<std::string, Double_t> eV = {
	{"Ti_ka", (4504.9 * 50 + 4510.8 * 100) / 150},
	{"Ti_kb", 4931.8},
	{"Cr_ka", (5405.5 * 50 + 5414.7 * 100) / 150},
	{"Mn_ka", (5887.6 * 50 + 5898.8 * 100) / 150},
	{"Fe_ka", (6390.8 * 50 + 6403.8 * 100) / 150},
	{"Fe_kb", 7058},
	{"Ni_ka", (7460.9 * 51 + 7478.2 * 100) / 151},
	{"Cu_ka", (8027.8 * 51 + 8047.8 * 100) / 151},
	{"Zn_ka", (8615.8 * 51 + 8638.9 * 100) / 151},
	{"Cu_kb", 8905.3},
	{"Zn_kb", 9572.0},
	{"Pb_la", (10551.5 * 100 + 10598.5 * 22) / 122},
	{"Bi_la", (10730.9 * 11 + 10838.8 * 100) / 111},
	{"Au_lb", (11442.3 * 67 + 11584.7 * 23) / 90},
	{"Br_ka", (11877.6 * 52 + 11924.2 * 100) / 152},
	{"Pb_lb", (12613.7 * 66 + 12622.6 * 25) / 91},
	{"Bi_lb", (12979.9 * 25 + 13023 * 67) / 92},
	{"Sr_ka", (14097.9 * 52 + 14165.0 * 100) / 152},
	{"Sr_kb", (15824.9 * 7 + 15835.7 * 14) / 21},
	{"Sr_kb2", 16084.6}};

Double_t eVEnergy[] = {eV["Ti_ka"], eV["Fe_ka"], eV["Cu_ka"], eV["Sr_ka"]};


// Initial sigma of sum of SDD spectrums
std::map<std::string, Double_t> sum_amp = {
	{"Ti_ka", 750000},
	{"Ti_kb", 100000},
	{"Cr_ka", 28000},
	{"Mn_ka", 15000},
	{"Fe_ka", 1550000},
	{"Fe_kb", 250000},
	{"Ni_ka", 15000},
	{"Cu_ka", 3900000},
	{"Zn_ka", 23000},
	{"Cu_kb", 600000},
	{"Zn_kb", 9000},
	{"Pb_la", 16000},
	{"Bi_la", 18000},
	{"Au_lb", 20000},
	{"Br_ka", 5000},
	{"Pb_lb", 4000},
	{"Bi_lb", 4500},
	{"Sr_ka", 2150000},
	{"Sr_kb", 370000},
	{"Sr_kb2", 30000}};

// Initial sigma of sum of SDD spectrums
std::map<std::string, Double_t> sum_sig = {
	{"Ti_ka", 70},
	{"Ti_kb", 75},
	{"Cr_ka", 100},
	{"Mn_ka", 120},
	{"Fe_ka", 90},
	{"Fe_kb", 90},
	{"Ni_ka", 130},
	{"Cu_ka", 90},
	{"Zn_ka", 125},
	{"Pb_la", 120},
	{"As_kb", 90},
	{"Cu_kb", 90},
	{"Zn_kb", 105},
	{"As_ka", 90},
	{"Bi_la", 110},
	{"Au_lb", 100},
	{"Br_ka", 80},
	{"Pb_lb", 85},
	{"Bi_lb", 90},
	{"Sr_ka", 90},
	{"Sr_kb", 90},
	{"Sr_kb2", 110}};

// Br/Kr_ka maybe escape peak of Sr

// Initial amp of tail functions of sum
std::map<std::string, Double_t> tail_amp = {
	{"Ti_ka", 50},
	{"Fe_ka", 50},
	{"Cu_ka", 50},
	{"Sr_ka", 50},
	{"Sr_kb", 50}};

// Initial beta of tail functions of sum
std::map<std::string, Double_t> tail_beta = {
	{"Ti_ka", 1.5},
	{"Fe_ka", 1.5},
	{"Cu_ka", 1.5},
	{"Sr_ka", 1.5},
	{"Sr_kb", 1.5}};