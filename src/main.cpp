#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include "interpreter.cpp"

using namespace geode;
using namespace cocos2d;
using geode::cocos::CCArrayExt;

class FunctionToolPopup : public FLAlertLayer, TextInputDelegate, FLAlertLayerProtocol {
public:
    TextInput* m_input_x;
    TextInput* m_input_y;
    TextInput* m_input_scale_x;
    TextInput* m_input_scale_y;
    TextInput* m_input_rotation;
    TextInput* m_input_n;
    TextInput* m_input_start;
    TextInput* m_input_end;
    TextInput* m_input_base_hue;
    TextInput* m_input_base_saturation;
    TextInput* m_input_base_value;
    TextInput* m_input_detail_hue;
    TextInput* m_input_detail_saturation;
    TextInput* m_input_detail_value;

	static auto* create() {
		auto* node = new (std::nothrow) FunctionToolPopup();
		if (node && node->init()) {
			node->autorelease();
		} else {
			delete node;
			node = nullptr;
		}
		return node;
	}

	bool init() override {
		if (!this->initWithColor({0, 0, 0, 75})) return false;

		this->m_noElasticity = true;

		auto* director = CCDirector::sharedDirector();
		geode::cocos::handleTouchPriority(this);
		this->registerWithTouchDispatcher();

		auto layer = CCLayer::create();
		auto menu = CCMenu::create();
		this->m_mainLayer = layer;
		this->m_buttonMenu = menu;
		
		layer->addChild(menu);
		this->addChild(layer);

		const float width = 445, height = 310;
		const CCPoint offset = director->getWinSize() / 2.f;
		auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
		bg->setContentSize({width, height});
		bg->setPosition(offset);
		menu->setPosition(offset);
		bg->setZOrder(-2);
		layer->addChild(bg);

		this->setKeypadEnabled(true);
        
        auto title_label = CCLabelBMFont::create("Function tool", "bigFont.fnt");
        title_label->setPosition(offset + ccp(0, 140));
        title_label->setScale(0.75f);
        layer->addChild(title_label);

        int input_width = 205;

        m_input_x = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_x->setLabel("Position x(t) =");
        m_input_x->setPosition(offset + ccp(-110, 100));
        m_input_x->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_x);

        m_input_y = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_y->setLabel("Position y(t) =");
        m_input_y->setPosition(offset + ccp(110, 100));
        m_input_y->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_y);

        m_input_scale_x = TextInput::create(input_width, "1.0", "chatFont.fnt");
        m_input_scale_x->setLabel("Scale x(t) =");
        m_input_scale_x->setPosition(offset + ccp(-110, 55));
        m_input_scale_x->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_scale_x);

        m_input_scale_y = TextInput::create(input_width, "1.0", "chatFont.fnt");
        m_input_scale_y->setLabel("Scale y(t) =");
        m_input_scale_y->setPosition(offset + ccp(110, 55));
        m_input_scale_y->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_scale_y);

        m_input_rotation = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_rotation->setLabel("Rotation(t) =");
        m_input_rotation->setPosition(offset + ccp(-110, 10));
        m_input_rotation->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_rotation);

        m_input_n = TextInput::create(60, "", "chatFont.fnt");
        m_input_n->setLabel("Amount: ");
        m_input_n->setString("20", false);
        m_input_n->setPosition(offset + ccp(40, 10));
        m_input_n->setCommonFilter(CommonFilter::Uint);
        this->addChild(m_input_n);

        m_input_start = TextInput::create(60, "", "chatFont.fnt");
        m_input_start->setLabel("Start t: ");
        m_input_start->setString("0.0", false);
        m_input_start->setPosition(offset + ccp(110, 10));
        m_input_start->setCommonFilter(CommonFilter::Float);
        this->addChild(m_input_start);

        m_input_end = TextInput::create(60, "", "chatFont.fnt");
        m_input_end->setLabel("End t: ");
        m_input_end->setString("5.0", false);
        m_input_end->setPosition(offset + ccp(180, 10));
        m_input_end->setCommonFilter(CommonFilter::Float);
        this->addChild(m_input_end);

        m_input_base_hue = TextInput::create(130, "0.0", "chatFont.fnt");
        m_input_base_hue->setLabel("Base hue(t) =");
        m_input_base_hue->setPosition(offset + ccp(-140, -35));
        m_input_base_hue->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_base_hue);

        m_input_base_saturation = TextInput::create(130, "0.0", "chatFont.fnt");
        m_input_base_saturation->setLabel("Base saturation(t) =");
        m_input_base_saturation->setPosition(offset + ccp(0, -35));
        m_input_base_saturation->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_base_saturation);

        m_input_base_value = TextInput::create(130, "0.0", "chatFont.fnt");
        m_input_base_value->setLabel("Base value(t) =");
        m_input_base_value->setPosition(offset + ccp(140, -35));
        m_input_base_value->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_base_value);

        m_input_detail_hue = TextInput::create(130, "0.0", "chatFont.fnt");
        m_input_detail_hue->setLabel("Detail hue(t) =");
        m_input_detail_hue->setPosition(offset + ccp(-140, -80));
        m_input_detail_hue->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_detail_hue);

        m_input_detail_saturation = TextInput::create(130, "0.0", "chatFont.fnt");
        m_input_detail_saturation->setLabel("Detail saturation(t) =");
        m_input_detail_saturation->setPosition(offset + ccp(0, -80));
        m_input_detail_saturation->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_detail_saturation);

        m_input_detail_value = TextInput::create(130, "0.0", "chatFont.fnt");
        m_input_detail_value->setLabel("Detail value(t) =");
        m_input_detail_value->setPosition(offset + ccp(140, -80));
        m_input_detail_value->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_detail_value);

		float button_width = 68;

        auto apply_button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Apply", button_width, true, "bigFont.fnt", "GJ_button_01.png", 0, 0.75f),
            this, menu_selector(FunctionToolPopup::on_apply)
        );
        apply_button->setPosition(button_width / 2.f + 20, -125);
		menu->addChild(apply_button);
            
        auto cancel_button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Cancel", button_width, true, "bigFont.fnt", "GJ_button_01.png", 0, 0.75f),
            this, menu_selector(FunctionToolPopup::on_close)
        );
        cancel_button->setPosition(-(button_width / 2.f + 20), -125);
		menu->addChild(cancel_button);

		this->setTouchEnabled(true);
		return true;
	}

	void keyBackClicked() override {
		this->setTouchEnabled(false);
		this->removeFromParentAndCleanup(true);
	}

	void on_close(CCObject*) {
		this->keyBackClicked();
	}

	void on_apply(CCObject*) {
		auto* editor = GameManager::sharedState()->getEditorLayer()->m_editorUI;
		auto objs = editor->getSelectedObjects();
		if (objs && objs->count()) {
			perform();
		}
	}

	void FLAlert_Clicked(FLAlertLayer*, bool btn2) override {
		if (btn2) perform();
	}

    CCArray* copyObjects(CCArray* objects) {
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

    void scaleRelative(GameObject* obj, CCPoint relative, float scale_x, float scale_y) {
        obj->setRScaleX(scale_x);
        obj->setRScaleY(scale_y);

        auto pos = obj->getRealPosition();

        pos -= relative;
        pos.x *= scale_x;
        pos.y *= scale_y;
        pos += relative;

        obj->setPosition(pos);
    }

    void rotateRelative(GameObject* obj, CCPoint relative, float angle_rel) {
        if (angle_rel < 10e-6) return;

        obj->setRRotation(angle_rel);

        auto pos = obj->getRealPosition() - relative;

        double pi = 3.141592;

        double radius = std::sqrt(double(pos.x)*double(pos.x) + double(pos.y)*double(pos.y));
        double angle = std::atan2(double(pos.y), double(pos.x));

        angle -= pi * angle_rel / 180.f;

        pos.x = float(radius * std::cos(angle));
        pos.y = float(radius * std::sin(angle));
        
        pos += relative;

        obj->setPosition(pos);
    }

	void perform() {
        #define parse_field(out, in, field_name, default) {\
            auto input = in->getString();\
            auto expr_result = this->parse_input(input, field_name, default);\
            if (expr_result.has_value()) {\
                out = std::move(expr_result.value());\
            } else {\
                return;\
            }\
        }

        #define zero { .kind = ExprKind::Literal, .value = 0.0 }
        #define one  { .kind = ExprKind::Literal, .value = 1.0 }

        Expr x_expr;                    parse_field(x_expr, m_input_x, "position x(t)", zero);
        Expr y_expr;                    parse_field(y_expr, m_input_y, "position x(t)", zero);
        Expr scale_x_expr;              parse_field(scale_x_expr, m_input_scale_x, "scale x(t)", one);
        Expr scale_y_expr;              parse_field(scale_y_expr, m_input_scale_y, "scale y(t)", one);
        Expr rotation_expr;             parse_field(rotation_expr, m_input_rotation, "rotation(t)", zero);
        Expr base_hue_expr;             parse_field(base_hue_expr, m_input_base_hue, "base hue(t)", zero);
        Expr base_saturation_expr;      parse_field(base_saturation_expr, m_input_base_saturation, "base saturation(t)", zero);
        Expr base_value_expr;           parse_field(base_value_expr, m_input_base_value, "base value(t)", zero);
        Expr detail_hue_expr;           parse_field(detail_hue_expr, m_input_detail_hue, "detail hue(t)", zero);
        Expr detail_saturation_expr;    parse_field(detail_saturation_expr, m_input_detail_saturation, "detail saturation(t)", zero);
        Expr detail_value_expr;         parse_field(detail_value_expr, m_input_detail_value, "detail value(t)", zero);

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
        auto first_pos = static_cast<GameObject*>(selected->objectAtIndex(0))->getRealPosition();
        float x_min = first_pos.x;
        float x_max = first_pos.x;
        float y_min = first_pos.y;
        float y_max = first_pos.y;
        for (int i = 1; i < selected->count(); i++) {
            auto pos = static_cast<GameObject*>(selected->objectAtIndex(i))->getRealPosition();
            x_min = std::min(x_min, pos.x);
            x_max = std::max(x_max, pos.x);
            y_min = std::min(y_min, pos.y);
            y_max = std::max(y_max, pos.y);
        }

        CCPoint center = ccp((x_min+x_max)/2.f, (y_min+y_max)/2.f);

		for (float i = 0; i < steps; i++) {
            float t = start + (end - start) * (float)i/(float)steps;

            CCPoint pos = ccp(30.0*x_expr.interpret(t), 30.0*y_expr.interpret(t));
            CCPoint current_center = pos+center;
            float rotation = rotation_expr.interpret(t);
            float scale_x = scale_x_expr.interpret(t);
            float scale_y = scale_y_expr.interpret(t);
            float base_hue = base_hue_expr.interpret(t);
            float base_saturation = base_saturation_expr.interpret(t);
            float base_value = base_value_expr.interpret(t);
            float detail_hue = detail_hue_expr.interpret(t);
            float detail_saturation = detail_saturation_expr.interpret(t);
            float detail_value = detail_value_expr.interpret(t);
            
            bool ok = true;
            for (auto val : {pos.x, pos.y, rotation, scale_x, scale_y, base_hue, base_saturation, base_value, detail_hue, detail_saturation, detail_value}) {
                if (std::isnan(val) || std::isinf(val)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            
            auto current = copyObjects(selected);
            for (int j = 0; j < current->count(); j++) {
                GameObject* obj = static_cast<GameObject*>(current->objectAtIndex(j));

                ui->moveObject(obj, pos);
                this->scaleRelative(obj, current_center, scale_x, scale_y);
                this->rotateRelative(obj, current_center, rotation);
                obj->m_baseColor->m_hsv.h += base_hue;
                obj->m_baseColor->m_hsv.s += base_saturation;
                obj->m_baseColor->m_hsv.v += base_value;
            }
			
			objs->addObjectsFromArray(current);
		}
		editor->m_undoObjects->addObject(UndoObject::createWithArray(objs, UndoCommand::Paste));
		ui->selectObjects(objs, /* ignoreSelectFilter: */ true);
		this->keyBackClicked();
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
