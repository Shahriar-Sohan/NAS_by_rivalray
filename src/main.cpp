#include <drogon/drogon.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <json/json.h>

std::filesystem::path getNasFolderPathFromConfig(){
    std::ifstream configFile("config.json");
    if(!configFile.is_open()){
        throw std::runtime_error("Could not open config.json");
    }
    Json::Value configJson;
    configFile >> configJson;

    if (!configJson.isMember("nasFolderPath") || !configJson["nasFolderPath"].isString()){
        throw std::runtime_error("nasFolderPath not found or invalid in config.json");
    }
    return std::filesystem::path(configJson["nasFolderPath"].asString());
}

int main() {
    using namespace drogon;

    std::filesystem::path folderPath;
    try{
        folderPath = getNasFolderPathFromConfig();
    } catch (const std::exception &e) {
        std::cerr << "Error reading config.json: " << e.what() << "\n";

        std::string homeDir = std::getenv("HOME");  // Get your home dir (e.g. /Users/r1v4l)
        folderPath = std::filesystem::path(homeDir) / "Desktop" / "NAS" / "temp_nas_storage";  // Default
    }

    app().registerHandler(
        "/",
        [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
            Json::Value data;
            data["message"] = "hello from sohan nigga";
            data["status"] = "success";

            auto resp = HttpResponse::newHttpJsonResponse(data);
            callback(resp);
        }
    );

    app().registerHandler(
        "/files",
        [folderPath](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback){
            Json::Value res;

            if(!std::filesystem::exists(folderPath)){
                res["error"] = "NAS folder path does not exist or is not a directory";
            }else{
                for(const auto &entry : std::filesystem::directory_iterator(folderPath)){
                    Json::Value fileInfo;
                    fileInfo["name"] = entry.path().filename().string();
                    fileInfo["is_directory"] = entry.is_directory();
                    res["files"].append(fileInfo);
                }
            }
            auto resp = HttpResponse::newHttpJsonResponse(res);
            callback(resp);
        }
    );

    app().addListener("0.0.0.0", 9001);

    std::cout << "Server is running and listening on port 9001\n";

    app().run();
}
