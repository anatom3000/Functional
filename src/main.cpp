#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include "nodes.hpp"
#include "interpreter.cpp"

using namespace geode;
using namespace cocos2d;
using geode::cocos::CCArrayExt;

class CircleToolPopup : public FLAlertLayer, TextInputDelegate, FLAlertLayerProtocol {
public:
    TextInput* m_input_x;
    TextInput* m_input_y;
    TextInput* m_input_n;
    TextInput* m_input_start;
    TextInput* m_input_end;

	static auto* create() {
		auto* node = new (std::nothrow) CircleToolPopup();
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
        
        layer->addChild(
            NodeFactory<CCLabelBMFont>::start("Function tool", "bigFont.fnt")
            .setPosition(ccp(0, 140) + offset)
            .setScale(0.75f)
        );
        
        m_input_x = TextInput::create(420, "", "chatFont.fnt");
        m_input_x->setLabel("x(t) =");
        m_input_x->setPosition(offset + ccp(0, 100));
        m_input_x->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_x);

        m_input_y = TextInput::create(420, "", "chatFont.fnt");
        m_input_y->setLabel("y(t) =");
        m_input_y->setPosition(offset + ccp(0, 55));
        m_input_y->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_y);

        m_input_n = TextInput::create(60, "", "chatFont.fnt");
        m_input_n->setLabel("Amount: ");
        m_input_n->setString("100", false);
        m_input_n->setPosition(offset + ccp(0, 10));
        m_input_n->setCommonFilter(CommonFilter::Uint);
        this->addChild(m_input_n);


        m_input_start = TextInput::create(60, "", "chatFont.fnt");
        m_input_start->setLabel("Start t: ");
        m_input_start->setString("0.0", false);
        m_input_start->setPosition(offset + ccp(-40, -35));
        m_input_start->setCommonFilter(CommonFilter::Float);
        this->addChild(m_input_start);

        m_input_end = TextInput::create(60, "", "chatFont.fnt");
        m_input_end->setLabel("End t: ");
        m_input_end->setString("5.0", false);
        m_input_end->setPosition(offset + ccp(40, -35));
        m_input_end->setCommonFilter(CommonFilter::Float);
        this->addChild(m_input_end);

		float button_width = 68;
		menu->addChild(
			make_factory(CCMenuItemSpriteExtra::create(
				ButtonSprite::create("Apply", button_width, true, "bigFont.fnt", "GJ_button_01.png", 0, 0.75f),
				this, menu_selector(CircleToolPopup::on_apply)
			))
			.setPosition(button_width / 2.f + 20, -125)
		);

		menu->addChild(
			make_factory(CCMenuItemSpriteExtra::create(
				ButtonSprite::create("Cancel", button_width, true, "bigFont.fnt", "GJ_button_01.png", 0, 0.75f),
				this, menu_selector(CircleToolPopup::on_close)
			))
			.setPosition(button_width / -2.f - 20, -125)
		);

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
		if (btn2)
			perform();
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

    std::optional<Expr> parse_input(std::string input, const char* field) {
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
            FLAlertLayer::create(
                "Error",
                std::format("{} field is empty", field),
                "OK"
            )->show();

            return std::nullopt;
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

	void perform() {
        auto x_input = m_input_x->getString();
        auto x_expr_result = this->parse_input(x_input, "X");

        Expr x_expr;
        if (x_expr_result.has_value()) {
            x_expr = std::move(x_expr_result.value());
        } else {
            return;
        }

        auto y_input = m_input_y->getString();
        auto y_expr_result = this->parse_input(y_input, "Y");

        Expr y_expr;
        if (y_expr_result.has_value()) {
            y_expr = std::move(y_expr_result.value());
        } else {
            return;
        }

        const auto x = [&](float t) -> float {
            return x_expr.interpret(t);
        };
        const auto y = [&](float t) -> float {
            return y_expr.interpret(t);
        };

		auto* editor = GameManager::sharedState()->getEditorLayer();
		EditorUI* ui = editor->m_editorUI;
		auto* objs = CCArray::create();

        auto selected = ui->getSelectedObjects()->shallowCopy();

        std::string steps_str = m_input_n->getString();
        if (steps_str.length() == 0) {
            FLAlertLayer::create(
                "Error",
                std::format("Amount field is empty"),
                "OK"
            )->show();
        }

        std::string start_str = m_input_start->getString();
        if (start_str.length() == 0) {
            FLAlertLayer::create(
                "Error",
                std::format("Start t field is empty"),
                "OK"
            )->show();
        }

        std::string end_str = m_input_end->getString();
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
		for (float i = 0; i < steps; i++) {
            float t = start + (end - start) * (float)i/(float)(steps-1);

            auto current = copyObjects(selected);
            CCPoint pos = ccp(30.0*x(t), 30.0*y(t));
            if (pos.x == 0.0 && pos.y == 0.0) continue;
            for (int j = 0; j < current->count(); j++) {
                GameObject* obj = static_cast<GameObject*>(current->objectAtIndex(j));

                ui->moveObject(obj, pos);
            }
			
			objs->addObjectsFromArray(current);
		}
		editor->m_undoObjects->addObject(UndoObject::createWithArray(objs, UndoCommand::Paste));
		ui->selectObjects(objs, /* ignoreSelectFilter: */ true);
		this->keyBackClicked();
	}
};

class $modify(MyEditorUI, EditorUI) {
	void on_circle_tool(CCObject*) {
		if (this->getSelectedObjects()->count()) {
			CircleToolPopup::create()->show();
		} else {
			FLAlertLayer::create("Info", "You must select some objects to use circle tool", "OK")->show();
		}
	}

	void createMoveMenu() {
		EditorUI::createMoveMenu();
		auto* btn = this->getSpriteButton("GJ_likeBtn_001.png", menu_selector(MyEditorUI::on_circle_tool), nullptr, 0.9f);
		m_editButtonBar->m_buttonArray->addObject(btn);
		auto rows = GameManager::sharedState()->getIntGameVariable("0049");
		auto cols = GameManager::sharedState()->getIntGameVariable("0050");
		// TODO: replace with reloadItemsInNormalSize
		m_editButtonBar->reloadItems(rows, cols);
	}
};
