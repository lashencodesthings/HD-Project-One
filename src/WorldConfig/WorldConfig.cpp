#include "WorldConfig.h"
#include "splashkit.h"

// Private parsers
static TerrainConfig parse_terrain(json j)
{
    return {
        json_read_number_as_double(j, "amplitude_ratio"),
        json_read_number_as_double(j, "base_height_ratio"),
        json_read_number_as_double(j, "big_scale"),
        json_read_number_as_double(j, "small_scale"),
        json_read_number_as_double(j, "small_weight")
    };
}

static NoiseConfig parse_noise(json j)
{
    return {
        json_read_number_as_int(j, "surface_offset"),
        json_read_number_as_int(j, "underground_offset")
    };
}

static LayerConfig parse_layer(json j)
{
    return {
        json_read_number_as_int(j, "thickness"),
        json_read_number_as_int(j, "variation"),
        json_read_number_as_double(j, "noise_scale")
    };
}

// Public function
WorldConfig load_world_config(const string &path)
{
    json j = json_from_file(path);

    WorldConfig config;
    config.terrain = parse_terrain(json_read_object(j, "terrain"));
    config.noise = parse_noise(json_read_object(j, "noise"));
    
    // Parse terrain and noise objects
    config.terrain = parse_terrain(json_read_object(j, "terrain"));
    config.noise = parse_noise(json_read_object(j, "noise"));

    std::vector<json> layers_array;
    json_read_array(j, "layers", layers_array);

    for (json layer_json : layers_array)
    {
        config.layers.push_back(parse_layer(layer_json));
    }

    return config;
}