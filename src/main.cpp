#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include "interpreter.cpp"

using namespace geode;
using namespace cocos2d;
using geode::cocos::CCArrayExt;

struct ToolConfig {
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


class FunctionToolPopup : public geode::Popup<> {
public:
    TextInput* m_input_x;
    TextInput* m_input_y;
    TextInput* m_input_rotation;

    TextInput* m_input_n;
    TextInput* m_input_start;
    TextInput* m_input_end;

    bool m_abs_scaling;
    bool m_abs_rotation;
    bool m_use_robtop_units;
    bool m_use_radians;
    bool m_delete_original;

    std::string m_scale_x;
    std::string m_scale_y;

    std::string m_base_hue;
    std::string m_base_saturation;
    std::string m_base_value;
    std::string m_detail_hue;
    std::string m_detail_saturation;
    std::string m_detail_value;

    #include "popups/settings.cpp"
    #include "popups/scale.cpp"
    #include "popups/hsv.cpp"


	static auto* create() {
		auto* node = new (std::nothrow) FunctionToolPopup();
		if (node && node->initAnchored(440, 180)) {
			node->autorelease();
		} else {
			delete node;
			node = nullptr;
		}
		return node;
	}

	bool setup() override {
        m_noElasticity = true;
        this->setTitle("Function tool");

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = winSize / 2.f;

        auto const center2 = CCSize(440, 180)/2;

        auto applySprite = ButtonSprite::create("Apply", "bigFont.fnt", "GJ_button_01.png", .60f);
        applySprite->setScale(0.85f);

        auto applyBtn = CCMenuItemSpriteExtra::create(
            applySprite,
            this,
            menu_selector(FunctionToolPopup::on_apply)
        );

        auto settingsSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        settingsSprite->setScale(0.7f);

        auto settingsBtn = CCMenuItemSpriteExtra::create(
            settingsSprite,
            this,
            menu_selector(FunctionToolPopup::onSettings)
        );
        auto scaleBtn = CCMenuItemSpriteExtra::create(
            EditorButtonSprite::createWithSpriteFrameName("edit_scaleXYBtn_001.png", .90f),
            this,
            menu_selector(FunctionToolPopup::onScale)
        );
        auto hsvBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_hsvBtn_001.png"),
            this,
            menu_selector(FunctionToolPopup::onHsv)
        );

        auto historyBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"),
            this,
            menu_selector(FunctionToolPopup::onScale)
        );

        applyBtn->setPosition(center2 + ccp(110, -63));
        settingsBtn->setPosition(center2 + ccp(-180, -63));
        scaleBtn->setPosition(center2 + ccp(-110, -63));
        hsvBtn->setPosition(center2 + ccp(-40, -63));
        historyBtn->setPosition(center2 + ccp(195, 65));

        m_buttonMenu->addChild(applyBtn);
        m_buttonMenu->addChild(settingsBtn);
        m_buttonMenu->addChild(scaleBtn);
        m_buttonMenu->addChild(hsvBtn);
        m_buttonMenu->addChild(historyBtn);

        int input_width = 200;

        m_input_x = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_x->setLabel("Position x(t) =");
        m_input_x->setPosition(center + ccp(-110, 25));
        m_input_x->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_x);

        m_input_y = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_y->setLabel("Position y(t) =");
        m_input_y->setPosition(center + ccp(110, 25));
        m_input_y->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_y);

        m_input_rotation = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_rotation->setLabel("Rotation(t) =");
        m_input_rotation->setPosition(center + ccp(-110, -25));
        m_input_rotation->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_rotation);

        m_input_n = TextInput::create(60, "", "chatFont.fnt");
        m_input_n->setLabel("Amount: ");
        m_input_n->setString("20", false);
        m_input_n->setPosition(center + ccp(40, -25));
        m_input_n->setCommonFilter(CommonFilter::Uint);
        this->addChild(m_input_n);

        m_input_start = TextInput::create(60, "", "chatFont.fnt");
        m_input_start->setLabel("Start t: ");
        m_input_start->setString("0.0", false);
        m_input_start->setPosition(center + ccp(110, -25));
        m_input_start->setCommonFilter(CommonFilter::Float);
        this->addChild(m_input_start);

        m_input_end = TextInput::create(60, "", "chatFont.fnt");
        m_input_end->setLabel("End t: ");
        m_input_end->setString("5.0", false);
        m_input_end->setPosition(center + ccp(180, -25));
        m_input_end->setCommonFilter(CommonFilter::Float);
        this->addChild(m_input_end);

		return true;
	}

	void on_apply(CCObject*) {
		auto* editor = GameManager::sharedState()->getEditorLayer()->m_editorUI;
		auto objs = editor->getSelectedObjects();
		if (objs && objs->count()) {
			perform();
		}
	}

    void onHsv(CCObject*) {
        auto sub = HsvPopup::create();
        sub->m_functool = this;
        
        sub->m_input_base_hue->setString(this->m_base_hue, false);
        sub->m_input_base_saturation->setString(this->m_base_saturation, false);
        sub->m_input_base_value->setString(this->m_base_value, false);

        sub->m_input_detail_hue->setString(this->m_detail_hue, false);
        sub->m_input_detail_saturation->setString(this->m_detail_saturation, false);
        sub->m_input_detail_value->setString(this->m_detail_value, false);

        sub->show();
    }

    void onScale(CCObject*) {
        auto sub = ScalePopup::create();
        sub->m_functool = this;

        sub->m_input_scale_x->setString(this->m_scale_x, false);
        sub->m_input_scale_y->setString(this->m_scale_y, false);

        sub->show();
    }

    void onSettings(CCObject*) {
        auto sub = SettingsPopup::create();
        sub->m_functool = this;

        sub->m_inputs[0]->toggle(m_abs_scaling);
        sub->m_inputs[1]->toggle(m_abs_rotation);
        sub->m_inputs[2]->toggle(m_use_robtop_units);
        sub->m_inputs[3]->toggle(m_use_radians);
        sub->m_inputs[4]->toggle(m_delete_original);

        sub->show();
    }

    CCArray* copyObjects(CCArray* objects) {
        // hack to copy objects using the in-game copy+paste

		auto* editor = GameManager::sharedState()->getEditorLayer();
		EditorUI* ui = editor->m_editorUI;

        auto selected = ui->getSelectedObjects()->shallowCopy();

        ui->deselectAll();
        ui->selectObjects(objects, false);

        ui->onDuplicate(nullptr);

        auto copied = ui->getSelectedObjects()->shallowCopy();
        ui->deselectAll();
        ui->selectObjects(selected, false);

        editor->m_undoObjects->removeLastObject();

        return copied;
    }

    std::optional<Expr> parse_input(std::string input, const char* field, Expr if_empty) {
        auto tokens_result = tokenize(input);

        if (tokens_result.index() != 0) {
            auto index = std::get<1>(tokens_result);

            FLAlertLayer::create(
                "Error",
                std::format("{}: unrecognized character '{}' at position {}", field, input[index], index+1),
                "OK"
            )->show();

            return std::nullopt;
        }

        auto tokens = std::get<std::vector<Token>>(tokens_result);

        if (tokens.size() == 0) {
            return if_empty;
        }

        auto expr_result = parse_expression(tokens);

        if (expr_result.index() != 0) {
            auto message = std::get<1>(expr_result);

            FLAlertLayer::create(
                "Error while parsing",
                std::format("{}: {}", field, message),
                "OK"
            )->show();

            return std::nullopt;
        }

        return std::get<0>(std::move(expr_result));
    }

    CCPoint centerOf(CCArray* objects) {
        auto first_pos = static_cast<GameObject*>(objects->objectAtIndex(0))->getRealPosition();

        float x_min = first_pos.x;
        float x_max = first_pos.x;
        float y_min = first_pos.y;
        float y_max = first_pos.y;
        for (int i = 1; i < objects->count(); i++) {
            auto pos = static_cast<GameObject*>(objects->objectAtIndex(i))->getRealPosition();
            x_min = std::min(x_min, pos.x);
            x_max = std::max(x_max, pos.x);
            y_min = std::min(y_min, pos.y);
            y_max = std::max(y_max, pos.y);
        }

        return ccp((x_min+x_max)/2.f, (y_min+y_max)/2.f);
    }

    void scaleRelative(GameObject* obj, CCPoint relative, float scale_x, float scale_y) {
		auto* editor = GameManager::sharedState()->getEditorLayer();
		EditorUI* ui = editor->m_editorUI;

        auto pos = obj->getRealPosition();
        ui->moveObject(obj, -pos);

        pos -= relative;
        pos.x *= scale_x;
        pos.y *= scale_y;
        pos += relative;
        
        ui->moveObject(obj, pos);
    }

	void perform() {
        #define parse_field(out, input, field_name, default)\
            Expr out;\
            {\
                auto expr_result = this->parse_input(input, field_name, default);\
                if (expr_result.has_value()) {\
                    out = std::move(expr_result.value());\
                } else {\
                    return;\
                }\
            }

        #define zero { .kind = ExprKind::Literal, .value = 0.0 }
        #define one  { .kind = ExprKind::Literal, .value = 1.0 }

        parse_field(x_expr,                 m_input_x->getString(),         "position x(t)",        zero);
        parse_field(y_expr,                 m_input_y->getString(),         "position x(t)",        zero);
        parse_field(scale_x_expr,           m_scale_x,                      "scale x(t)",           one );
        parse_field(scale_y_expr,           m_scale_y,                      "scale y(t)",           one );
        parse_field(rotation_expr,          m_input_rotation->getString(),  "rotation(t)",          zero);
        parse_field(base_hue_expr,          m_base_hue,                     "base hue(t)",          zero);
        parse_field(base_saturation_expr,   m_base_saturation,              "base saturation(t)",   zero);
        parse_field(base_value_expr,        m_base_value,                   "base value(t)",        zero);
        parse_field(detail_hue_expr,        m_detail_hue,                   "detail hue(t)",        zero);
        parse_field(detail_saturation_expr, m_detail_saturation,            "detail saturation(t)", zero);
        parse_field(detail_value_expr,      m_detail_value,                 "detail value(t)",      zero);

        std::string steps_str = m_input_n->getString();
        steps_str.erase(std::remove_if(steps_str.begin(), steps_str.end(), isspace), steps_str.end());

        if (steps_str.length() == 0) {
            FLAlertLayer::create(
                "Error",
                std::format("Amount field is empty"),
                "OK"
            )->show();
        }

        std::string start_str = m_input_start->getString();
        start_str.erase(std::remove_if(start_str.begin(), start_str.end(), isspace), start_str.end());
        if (start_str.length() == 0) {
            FLAlertLayer::create(
                "Error",
                std::format("Start t field is empty"),
                "OK"
            )->show();
        }

        std::string end_str = m_input_end->getString();
        end_str.erase(std::remove_if(end_str.begin(), end_str.end(), isspace), end_str.end());
        if (start_str.length() == 0) {
            FLAlertLayer::create(
                "Error",
                std::format("End t field is empty"),
                "OK"
            )->show();
        }

        int steps = std::stoi(steps_str);
        float start = std::stof(start_str);
        float end = std::stof(end_str);

		auto* editor = GameManager::sharedState()->getEditorLayer();
		EditorUI* ui = editor->m_editorUI;
		auto* objs = CCArray::create();

        auto selected = ui->getSelectedObjects()->shallowCopy();
        auto center = this->centerOf(selected);

		for (float i = 0; i < steps; i++) {
            float t = start + (end - start) * (float)i/(float)steps;

            InterpreterConfig c = {
                .t = t,
                .use_radians = m_use_radians,
            };
            
            float factor = m_use_robtop_units ? 1.0 : 30.0;
            CCPoint pos = ccp(factor * x_expr.interpret(c), factor * y_expr.interpret(c));
            float rotation = rotation_expr.interpret(c);
            float scale_x = scale_x_expr.interpret(c);
            float scale_y = scale_y_expr.interpret(c);
            float base_hue = base_hue_expr.interpret(c);
            float base_saturation = base_saturation_expr.interpret(c);
            float base_value = base_value_expr.interpret(c);
            float detail_hue = detail_hue_expr.interpret(c);
            float detail_saturation = detail_saturation_expr.interpret(c);
            float detail_value = detail_value_expr.interpret(c);
            
            bool ok = true;
            for (auto val : {pos.x, pos.y, rotation, scale_x, scale_y, base_hue, base_saturation, base_value, detail_hue, detail_saturation, detail_value}) {
                if (std::isnan(val) || std::isinf(val)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            
            auto current = copyObjects(selected);
            auto current_center = center+pos;

            for (int j = 0; j < current->count(); j++) {
                GameObject* obj = static_cast<GameObject*>(current->objectAtIndex(j));
                
                ui->moveObject(obj, ccp(pos.x, pos.y));
                
                auto obj_single = CCArray::createWithObject(obj);

        
                ui->scaleObjects(obj_single, scale_x*obj->getRScaleX(), scale_y*obj->getRScaleY(), current_center, ObjectScaleType::XY, /* absoluteScaling: */ true);
                if (!m_abs_scaling) this->scaleRelative(obj, current_center, scale_x, scale_y);
                if (m_abs_rotation) ui->rotateObjects(obj_single, rotation, obj->getRealPosition());

                obj->m_baseColor->m_hsv.h += base_hue;
                obj->m_baseColor->m_hsv.s += base_saturation;
                obj->m_baseColor->m_hsv.v += base_value;
            }
            
            if (!m_abs_rotation) ui->rotateObjects(current, rotation, current_center);
            
			objs->addObjectsFromArray(current);
		}
		editor->m_undoObjects->addObject(UndoObject::createWithArray(objs, UndoCommand::Paste));

        if (m_delete_original) ui->onDeleteSelected(nullptr);

		ui->selectObjects(objs, /* ignoreSelectFilter: */ true);
		this->keyBackClicked();
	}

    ToolConfig saveConfig() {
        return {
            .x = m_input_x->getString(),
            .y = m_input_y->getString(),
            .rotation = m_input_rotation->getString(),

            .n = m_input_n->getString(),
            .start = m_input_start->getString(),
            .end = m_input_end->getString(),

            .abs_scaling = m_abs_scaling,
            .abs_rotation = m_abs_rotation,
            .use_robtop_units = m_use_robtop_units,
            .use_radians = m_use_radians,
            .delete_original = m_delete_original,

            .scale_x = m_scale_x,
            .scale_y = m_scale_y,

            .base_hue = m_base_hue,
            .base_saturation = m_base_saturation,
            .base_value = m_base_value,
            .detail_hue = m_detail_hue,
            .detail_saturation = m_detail_saturation,
            .detail_value = m_detail_value

        };
    }

    void loadConfig(ToolConfig c) {
        m_input_x->setString(c.x, false);
        m_input_y->setString(c.y, false);
        m_input_rotation->setString(c.rotation, false);

        m_input_n->setString(c.n, false);
        m_input_start->setString(c.start, false);
        m_input_end->setString(c.end, false);

        m_abs_scaling = c.abs_scaling;
        m_abs_rotation = c.abs_rotation;
        m_use_robtop_units = c.use_robtop_units;
        m_use_radians = c.use_radians;
        m_delete_original = c.delete_original;

        m_scale_x = c.scale_x;
        m_scale_y = c.scale_y;

        m_base_hue = c.base_hue;
        m_base_saturation = c.base_saturation;
        m_base_value = c.base_value;
        m_detail_hue = c.detail_hue;
        m_detail_saturation = c.detail_saturation;
        m_detail_value = c.detail_value;
    }
};

class $modify(MyEditorUI, EditorUI) {
	void on_function_tool(CCObject*) {
		if (this->getSelectedObjects()->count()) {
			FunctionToolPopup::create()->show();
		} else {
			FLAlertLayer::create("Info", "You must select some objects to use the function tool.", "OK")->show();
		}
	}

	void createMoveMenu() {
		EditorUI::createMoveMenu();
		auto* btn = this->getSpriteButton("gj22_anim_64_color_004.png", menu_selector(MyEditorUI::on_function_tool), nullptr, 0.9f);
		m_editButtonBar->m_buttonArray->addObject(btn);
		auto rows = GameManager::sharedState()->getIntGameVariable("0049");
		auto cols = GameManager::sharedState()->getIntGameVariable("0050");
		// TODO: replace with reloadItemsInNormalSize
		m_editButtonBar->reloadItems(rows, cols);
	}
};
