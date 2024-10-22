class SettingsPopup : public geode::Popup<> {
public:
    FunctionToolPopup* m_functool;
    std::vector<CCMenuItemToggler*> m_inputs;
    CCMenuItemToggler* m_input_abs_scaling;
    CCMenuItemToggler* m_input_abs_rotation;

    static auto* create() {
        auto* node = new (std::nothrow) SettingsPopup();
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
        this->setTitle("Options");

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = winSize / 2.f;

        auto const center2 = CCSize(220, 180) / 2;

        int base_height = 38;

        #define entry(i, name, has_info) {\
            auto input = m_input_abs_scaling = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.6f);\
            input->setPosition(center2 + ccp(-72, base_height-24*i));\
            m_buttonMenu->addChild(input);\
            m_inputs.push_back(input);\
            auto label = CCLabelBMFont::create(name, "bigFont.fnt");\
            label->setScale(0.4f);\
            label->setPosition(center2 + ccp(-56.0+label->getScaledContentSize().width/2.0, base_height+1-24*i));\
            m_buttonMenu->addChild(label);\
            if (has_info) {\
                auto bubble = CCMenuItemSpriteExtra::create(\
                    bubbleSprite,\
                    this,\
                    menu_selector(SettingsPopup::bubbleCallback)\
                );\
                bubble->setTag(i);\
                bubble->setPosition(center2 + ccp(-94, base_height-24*i));\
                m_buttonMenu->addChild(bubble);\
            }\
        }

        auto bubbleSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        bubbleSprite->setScale(0.5f);

        entry(0, "Absolute scaling", false);
        entry(1, "Absolute rotation", false);
        entry(2, "Use Robtop units", true);
        entry(3, "Use radians", true);
        entry(4, "Delete original", true);

        return true;
    }

    void bubbleCallback(CCObject* sender) {
        const char* message = nullptr;
        switch (sender->getTag()) {
            case 2: message = "Use 1.0 = 1/30 blocks = 1 unit \nin position fields \ninstead of 1.0 = 1 block."; break;
            case 3: message = "Use radians instead of degrees \nin trig functions and in the rotation field."; break;
            case 4: message = "Delete the selected objects after generating copies."; break;
        }
        
        if (message == nullptr) return;
    
        FLAlertLayer::create(
            "Info",
            message,
            "OK"\
        )->show();

    }

    void onClose(CCObject* obj) override {
        m_functool->m_abs_scaling      = m_inputs[0]->isToggled();
        m_functool->m_abs_rotation     = m_inputs[1]->isToggled();
        m_functool->m_use_robtop_units = m_inputs[2]->isToggled();
        m_functool->m_use_radians      = m_inputs[3]->isToggled();
        m_functool->m_delete_original  = m_inputs[4]->isToggled();
        Popup::onClose(obj);
    }
};
