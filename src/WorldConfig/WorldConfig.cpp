#include "WorldConfig.h"

#include <fstream>
#include "../../libraries/json.hpp"

using json = nlohmann::json;

// Json Loading
json load_json(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("Failed to open world config file");
    }

    json j;
    file >> j; // Overloaded operator by Nlohmann that parses the read JSON text into the json_text variable
    return j;
}

// Private parsers for parsing the externally read json text into the program's config values
static TerrainConfig parse_terrain(const json& j)
{
    return {
        j["amplitude_ratio"],
        j["base_height_ratio"],
        j["big_scale"],
        j["small_scale"],
        j["small_weight"]
    };
}

static NoiseConfig parse_noise(const json& j)
{
    return {
        j["surface_offset"],
        j["underground_offset"]
    };
}

static LayerConfig parse_layer(const json& j)
{
    return {
        j["thickness"],
        j["variation"],
        j["noise_scale"],
        (BlockType)(j["block"]),
        (WallType)(j["wall"])
    };
}

// Public function for returning a world config from the extracted json and private parser values
WorldConfig load_world_config(const std::string& path)
{
    json j = load_json(path);

    WorldConfig config;
    config.terrain = parse_terrain(j["terrain"]);
    config.noise = parse_noise(j["noise"]);

    for (const auto& layer_json : j["layers"])
    {
        config.layers.push_back(parse_layer(layer_json));
    }

    return config;
}