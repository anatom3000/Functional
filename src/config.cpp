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
    static ToolConfig from_json(matjson::Value const& value) {
        return {
            .name = value["name"].as_string(),
            .x = value["x"].as_string(),
            .y = value["y"].as_string(),
            .rotation = value["rotation"].as_string(),

            .n = value["n"].as_string(),
            .start = value["start"].as_string(),
            .end = value["end"].as_string(),

            .abs_scaling = value["abs_scaling"].as_bool(),
            .abs_rotation = value["abs_rotation"].as_bool(),
            .use_robtop_units = value["use_robtop_units"].as_bool(),
            .use_radians = value["use_radians"].as_bool(),
            .delete_original = value["delete_original"].as_bool(),

            .scale_x = value["scale_x"].as_string(),
            .scale_y = value["scale_y"].as_string(),

            .base_hue = value["base_hue"].as_string(),
            .base_saturation = value["base_saturation"].as_string(),
            .base_value = value["base_value"].as_string(),
            .detail_hue = value["detail_hue"].as_string(),
            .detail_saturation = value["detail_saturation"].as_string(),
            .detail_value = value["detail_value"].as_string()
        };
    }

    static matjson::Value to_json(ToolConfig const& config) {
        auto value = matjson::Object();
        
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

#define DEFAULT_PRESETS\
    ToolConfig {\
        .name = "Circle",\
        .x = "4*sin(t)",\
        .y = "4*cos(t)",\
        .rotation = "t",\
        .n = "16",\
        .start = "0",\
        .end = "360",\
        .delete_original = true\
    },\
    ToolConfig {\
        .name = "Random blocks",\
        .x = "round(20*random())",\
        .y = "round(20*random())",\
        .n = "50",\
        .start = "0.0",\
        .end = "1.0",\
        .delete_original = true\
    },\
