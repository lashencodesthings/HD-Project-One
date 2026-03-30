#include "WorldConfig.h"
#include "splashkit.h"

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

static TerrainSnakesConfig parse_terrain_snakes(json j)
{
    return {
        json_read_number_as_int(j, "VEIN_SPACING"),
        json_read_number_as_int(j, "VEINS_PER_CLUSTER"),
        json_read_number_as_int(j, "VEIN_COUNT_VARIATION"),
        json_read_number_as_int(j, "MIN_VEIN_LENGTH"),
        json_read_number_as_int(j, "VEIN_LENGTH_VARIATION"),
        json_read_number_as_int(j, "MIN_BRUSH_RADIUS"),
        json_read_number_as_int(j, "BRUSH_RADIUS_VARIATION"),
        json_read_number_as_int(j, "MOVE_RANGE"),
        json_read_number_as_int(j, "MOVE_OFFSET"),
        json_read_number_as_double(j, "STEP_SCALE"),
        json_read_number_as_int(j, "SURFACE_OFFSET"),
        json_read_number_as_int(j, "BRANCH_PROBABILITY"),
        json_read_number_as_int(j, "MAX_BRANCHES"),
    };
}

WorldConfig load_world_config(const string &path)
{
    json j = json_from_file(path);

    WorldConfig config;
    config.terrain = parse_terrain(json_read_object(j, "terrain"));
    config.noise = parse_noise(json_read_object(j, "noise"));
    config.terrain_snakes = parse_terrain_snakes(json_read_object(j, "terrain_snakes"));

    std::vector<json> layers_array;
    json_read_array(j, "layers", layers_array);

    for (json layer_json : layers_array)
    {
        config.layers.push_back(parse_layer(layer_json));
    }

    return config;
}