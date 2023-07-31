#include <nlohmann/json.hpp>


void saveToJson(std::vector<calibrationSettings> calibrationSettingsList)
{
    nlohmann::json jsonObject;
    nlohmann::json calibrationSettingsArray = nlohmann::json::array();

    for (const auto &calibrationSettings : calibrationSettingsList)
    {
        nlohmann::json calibrationSettingsObject;
        calibrationSettingsObject["sdd_id"] = calibrationSettings.sdd_id;
        calibrationSettingsObject["slope"] = calibrationSettings.slope;
        calibrationSettingsObject["intercept"] = calibrationSettings.intercept;
        calibrationSettingsArray.push_back(calibrationSettingsObject);
    }
    jsonObject["calibration_settings"] = calibrationSettingsArray;

    std::string jsonString = jsonObject.dump(4);

    std::ofstream outputFile("output/output.json");
    if (outputFile.is_open())
    {
        outputFile << jsonString;
        outputFile.close();
        std::cout << "JSON file written successfully." << std::endl;
    }
    else
    {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }
}