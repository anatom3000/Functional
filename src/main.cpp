#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include "interpreter.cpp"

using namespace geode;
using namespace cocos2d;
using geode::cocos::CCArrayExt;


class FunctionToolPopup : public geode::Popup<> {
public:
    TextInput* m_input_x;
    TextInput* m_input_y;
    std::string m_scale_x;
    std::string m_scale_y;
    TextInput* m_input_rotation;
    TextInput* m_input_n;
    TextInput* m_input_start;
    TextInput* m_input_end;
    std::string m_base_hue;
    std::string m_base_saturation;
    std::string m_base_value;
    std::string m_detail_hue;
    std::string m_detail_saturation;
    std::string m_detail_value;


    class ScalePopup : public geode::Popup<> {
    public:
        TextInput* m_input_scale_x;
        TextInput* m_input_scale_y;
        FunctionToolPopup* m_functool;

        static auto* create() {
            auto* node = new (std::nothrow) ScalePopup();
            if (node && node->initAnchored(220, 180)) {
                node->autorelease();
            } else {
                delete node;
                node = nullptr;
            }
            return node;
        }

        bool setup() override {
            m_noElasticity = true;
            this->setTitle("Scale");

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto center = winSize / 2.f;

            auto const center2 = CCSize(220, 280) / 2;

            auto okBtn = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("OK", "bigFont.fnt", "GJ_button_01.png", .75f),
                this,
                menu_selector(HsvPopup::onClose)
            );
            okBtn->setPosition(center2 + ccp(0, -112));
            m_buttonMenu->addChild(okBtn);

            int input_width = 200;

            m_input_scale_x = TextInput::create(input_width, "1.0", "chatFont.fnt");
            m_input_scale_x->setLabel("Scale x(t) =");
            m_input_scale_x->setPosition(center + ccp(0, 30));
            m_input_scale_x->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_scale_x);

            m_input_scale_y = TextInput::create(input_width, "1.0", "chatFont.fnt");
            m_input_scale_y->setLabel("Scale y(t) =");
            m_input_scale_y->setPosition(center + ccp(0, -20));
            m_input_scale_y->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_scale_y);

            return true;
        }

        void onClose(CCObject* obj) override {
            this->m_functool->m_scale_x = this->m_input_scale_x->getString();
            this->m_functool->m_scale_y = this->m_input_scale_y->getString();

            Popup::onClose(obj);
        }
    };


    class HsvPopup : public geode::Popup<> {
    public:
        TextInput* m_input_base_hue;
        TextInput* m_input_base_saturation;
        TextInput* m_input_base_value;
        TextInput* m_input_detail_hue;
        TextInput* m_input_detail_saturation;
        TextInput* m_input_detail_value;
        FunctionToolPopup* m_functool;

        static auto* create() {
            auto* node = new (std::nothrow) HsvPopup();
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
            this->setTitle("HSV");

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto center = winSize / 2.f;

            auto const center2 = CCSize(440, 280) / 2;

            auto okBtn = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("OK", "bigFont.fnt", "GJ_button_01.png", .75f),
                this,
                menu_selector(HsvPopup::onClose)
            );
            okBtn->setPosition(center2 + ccp(0, -112));
            m_buttonMenu->addChild(okBtn);

            int input_width = 205;

            m_input_base_hue = TextInput::create(130, "0.0", "chatFont.fnt");
            m_input_base_hue->setLabel("Base hue(t) =");
            m_input_base_hue->setPosition(center + ccp(-140, 25));
            m_input_base_hue->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_base_hue);

            m_input_base_saturation = TextInput::create(130, "0.0", "chatFont.fnt");
            m_input_base_saturation->setLabel("Base saturation(t) =");
            m_input_base_saturation->setPosition(center + ccp(0, 25));
            m_input_base_saturation->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_base_saturation);

            m_input_base_value = TextInput::create(130, "0.0", "chatFont.fnt");
            m_input_base_value->setLabel("Base value(t) =");
            m_input_base_value->setPosition(center + ccp(140, 25));
            m_input_base_value->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_base_value);

            m_input_detail_hue = TextInput::create(130, "0.0", "chatFont.fnt");
            m_input_detail_hue->setLabel("Detail hue(t) =");
            m_input_detail_hue->setPosition(center + ccp(-140, -25));
            m_input_detail_hue->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_detail_hue);

            m_input_detail_saturation = TextInput::create(130, "0.0", "chatFont.fnt");
            m_input_detail_saturation->setLabel("Detail saturation(t) =");
            m_input_detail_saturation->setPosition(center + ccp(0, -25));
            m_input_detail_saturation->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_detail_saturation);

            m_input_detail_value = TextInput::create(130, "0.0", "chatFont.fnt");
            m_input_detail_value->setLabel("Detail value(t) =");
            m_input_detail_value->setPosition(center + ccp(140, -25));
            m_input_detail_value->setCommonFilter(CommonFilter::Any);
            this->addChild(m_input_detail_value);


            return true;
        }

        void onClose(CCObject* obj) override {
            this->m_functool->m_base_hue = this->m_input_base_hue->getString();
            this->m_functool->m_base_saturation = this->m_input_base_saturation->getString();
            this->m_functool->m_base_value = this->m_input_base_value->getString();

            this->m_functool->m_detail_hue = this->m_input_detail_hue->getString();
            this->m_functool->m_detail_saturation = this->m_input_detail_saturation->getString();
            this->m_functool->m_detail_value = this->m_input_detail_value->getString();

            Popup::onClose(obj);
        }
    };

	static auto* create() {
		auto* node = new (std::nothrow) FunctionToolPopup();
		if (node && node->initAnchored(440, 280)) {
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

        auto const center2 = CCSize(440, 280)/2;

        auto applyBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Apply", "bigFont.fnt", "GJ_button_01.png", .75f),
            this,
            menu_selector(FunctionToolPopup::on_apply)
        );
        auto scaleBtn = CCMenuItemSpriteExtra::create(
            EditorButtonSprite::createWithSpriteFrameName("edit_scaleXYBtn_001.png", .95f),
            this,
            menu_selector(FunctionToolPopup::onScale)
        );
        auto hsvBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_hsvBtn_001.png"),
            this,
            menu_selector(FunctionToolPopup::onHsv)
        );

        applyBtn->setPosition(center2 + ccp(0, -110));
        scaleBtn->setPosition(center2 + ccp(110, -20));
        hsvBtn->setPosition(center2 + ccp(180, -20));
        m_buttonMenu->addChild(applyBtn);
        m_buttonMenu->addChild(scaleBtn);
        m_buttonMenu->addChild(hsvBtn);

        int input_width = 200;

        m_input_x = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_x->setLabel("Position x(t) =");
        m_input_x->setPosition(center + ccp(-110, 80));
        m_input_x->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_x);

        m_input_y = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_y->setLabel("Position y(t) =");
        m_input_y->setPosition(center + ccp(110, 80));
        m_input_y->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_y);

        m_input_rotation = TextInput::create(input_width, "0.0", "chatFont.fnt");
        m_input_rotation->setLabel("Rotation(t) =");
        m_input_rotation->setPosition(center + ccp(-110, -15));
        m_input_rotation->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_rotation);

        m_input_n = TextInput::create(60, "", "chatFont.fnt");
        m_input_n->setLabel("Amount: ");
        m_input_n->setString("20", false);
        m_input_n->setPosition(center + ccp(40, -70));
        m_input_n->setCommonFilter(CommonFilter::Uint);
        this->addChild(m_input_n);

        m_input_start = TextInput::create(60, "", "chatFont.fnt");
        m_input_start->setLabel("Start t: ");
        m_input_start->setString("0.0", false);
        m_input_start->setPosition(center + ccp(110, -70));
        m_input_start->setCommonFilter(CommonFilter::Float);
        this->addChild(m_input_start);

        m_input_end = TextInput::create(60, "", "chatFont.fnt");
        m_input_end->setLabel("End t: ");
        m_input_end->setString("5.0", false);
        m_input_end->setPosition(center + ccp(180, -70));
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
		auto* editor = GameManager::sharedState()->getEditorLayer();
		EditorUI* ui = editor->m_editorUI;

        auto pos = obj->getRealPosition();

        pos -= relative;
        pos.x *= scale_x;
        pos.y *= scale_y;
        pos += relative;

        obj->setPosition(pos);
    }

    void rotateRelative(GameObject* obj, CCPoint relative, float angle_rel) {
        if (angle_rel < 10e-6) return;

		auto* editor = GameManager::sharedState()->getEditorLayer();
		EditorUI* ui = editor->m_editorUI;

        obj->setRRotation(angle_rel);

        auto pos = obj->getRealPosition();
        auto pi = 3.141592;

        ui->moveObject(obj, -pos);

        pos -= relative;

        auto c = std::cos(pi * angle_rel / 180.f);
        auto s = std::sin(pi * angle_rel / 180.f);

        auto x = pos.x = pos.x * c - pos.y * s;
        auto y = pos.x * s + pos.y * c;

        pos.x = x;
        pos.y = y;
        
        pos += relative;
        ui->moveObject(obj, pos);
        
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

	void perform() {
        #define parse_field(out, input, field_name, default) {\
            auto expr_result = this->parse_input(input, field_name, default);\
            if (expr_result.has_value()) {\
                out = std::move(expr_result.value());\
            } else {\
                return;\
            }\
        }

        #define zero { .kind = ExprKind::Literal, .value = 0.0 }
        #define one  { .kind = ExprKind::Literal, .value = 1.0 }

        Expr x_expr;                    parse_field(x_expr, m_input_x->getString(), "position x(t)", zero);
        Expr y_expr;                    parse_field(y_expr, m_input_y->getString(), "position x(t)", zero);
        Expr scale_x_expr;              parse_field(scale_x_expr, m_scale_x, "scale x(t)", one);
        Expr scale_y_expr;              parse_field(scale_y_expr, m_scale_y, "scale y(t)", one);
        Expr rotation_expr;             parse_field(rotation_expr, m_input_rotation->getString(), "rotation(t)", zero);
        Expr base_hue_expr;             parse_field(base_hue_expr, m_base_hue, "base hue(t)", zero);
        Expr base_saturation_expr;      parse_field(base_saturation_expr, m_base_saturation, "base saturation(t)", zero);
        Expr base_value_expr;           parse_field(base_value_expr, m_base_value, "base value(t)", zero);
        Expr detail_hue_expr;           parse_field(detail_hue_expr, m_detail_hue, "detail hue(t)", zero);
        Expr detail_saturation_expr;    parse_field(detail_saturation_expr, m_detail_saturation, "detail saturation(t)", zero);
        Expr detail_value_expr;         parse_field(detail_value_expr, m_detail_value, "detail value(t)", zero);

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

            CCPoint pos = ccp(30.0*x_expr.interpret(t), 30.0*y_expr.interpret(t));
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
            auto current_center = center+pos;
            ui->transformObjects(current, current_center, scale_x, scale_y, 0.f, 0.f, 0.f, 0.f);
            for (int j = 0; j < current->count(); j++) {
                GameObject* obj = static_cast<GameObject*>(current->objectAtIndex(j));
                GameObject* first = static_cast<GameObject*>(current->objectAtIndex(0));

                ui->moveObject(obj, current_center+ccp(-current_center.x + scale_x*current_center.x, -current_center.y + scale_y*current_center.y));
                this->rotateRelative(obj, current_center, rotation);
                //this->scaleRelative(obj, current_center, scale_x, scale_y);
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
