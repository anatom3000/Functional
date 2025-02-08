struct ToolConfig {
    std::string name;

    std::string x;
    std::string y;
    std::string rotation;

    std::string n;
    std::string start;
    std::string end;

    bool abs_scaling;
    bool abs_rotation;
    bool use_robtop_units;
    bool use_radians;
    bool delete_original;

    std::string scale_x;
    std::string scale_y;

    std::string base_hue;
    std::string base_saturation;
    std::string base_value;
    std::string detail_hue;
    std::string detail_saturation;
    std::string detail_value;
};

template<>
struct matjson::Serialize<ToolConfig> {
    static Result<ToolConfig> fromJson(matjson::Value const& value) {
        ToolConfig config = {
            .name = GEODE_UNWRAP(value["name"].asString()),
            .x = GEODE_UNWRAP(value["x"].asString()),
            .y = GEODE_UNWRAP(value["y"].asString()),
            .rotation = GEODE_UNWRAP(value["rotation"].asString()),

            .n = GEODE_UNWRAP(value["n"].asString()),
            .start = GEODE_UNWRAP(value["start"].asString()),
            .end = GEODE_UNWRAP(value["end"].asString()),

            .abs_scaling = GEODE_UNWRAP(value["abs_scaling"].asBool()),
            .abs_rotation = GEODE_UNWRAP(value["abs_rotation"].asBool()),
            .use_robtop_units = GEODE_UNWRAP(value["use_robtop_units"].asBool()),
            .use_radians = GEODE_UNWRAP(value["use_radians"].asBool()),
            .delete_original = GEODE_UNWRAP(value["delete_original"].asBool()),

            .scale_x = GEODE_UNWRAP(value["scale_x"].asString()),
            .scale_y = GEODE_UNWRAP(value["scale_y"].asString()),

            .base_hue = GEODE_UNWRAP(value["base_hue"].asString()),
            .base_saturation = GEODE_UNWRAP(value["base_saturation"].asString()),
            .base_value = GEODE_UNWRAP(value["base_value"].asString()),
            .detail_hue = GEODE_UNWRAP(value["detail_hue"].asString()),
            .detail_saturation = GEODE_UNWRAP(value["detail_saturation"].asString()),
            .detail_value = GEODE_UNWRAP(value["detail_value"].asString())
        };

        return Ok(config);
    }

    static matjson::Value toJson(ToolConfig const& config) {
        auto value = matjson::Value();
        
        value["name"] = config.name;
        value["x"] = config.x;
        value["y"] = config.y;
        value["rotation"] = config.rotation;

        value["n"] = config.n;
        value["start"] = config.start;
        value["end"] = config.end;

        value["abs_scaling"] = config.abs_scaling;
        value["abs_rotation"] = config.abs_rotation;
        value["use_robtop_units"] = config.use_robtop_units;
        value["use_radians"] = config.use_radians;
        value["delete_original"] = config.delete_original;

        value["scale_x"] = config.scale_x;
        value["scale_y"] = config.scale_y;

        value["base_hue"] = config.base_hue;
        value["base_saturation"] = config.base_saturation;
        value["base_value"] = config.base_value;
        value["detail_hue"] = config.detail_hue;
        value["detail_saturation"] = config.detail_saturation;
        value["detail_value"] = config.detail_value;

        return value;
    }
};

#define DEFAULT_PRESETS             \
    ToolConfig {                    \
        .name = "Circle",           \
        .x = "4*sin(t)",            \
        .y = "4*cos(t)",            \
        .rotation = "t",            \
        .n = "16",                  \
        .start = "0",               \
        .end = "360",               \
        .delete_original = true     \
    },                              \
    ToolConfig {                    \
        .name = "Random blocks",    \
        .x = "round(20*random())",  \
        .y = "round(20*random())",  \
        .n = "50",                  \
        .start = "0.0",             \
        .end = "1.0",               \
        .delete_original = true     \
    },                              \
    ToolConfig {                    \
        .name = "Random blocks",    \
        .x = "round(20*random())",  \
        .y = "round(20*random())",  \
        .n = "50",                  \
        .start = "0.0",             \
        .end = "1.0",               \
        .delete_original = true     \
    },                              \
    ToolConfig {                    \
        .name = "Modify one",       \
        .n = "1",                   \
        .start = "0.0",             \
        .end = "1.0",               \
        .delete_original = true     \
    },                              \

