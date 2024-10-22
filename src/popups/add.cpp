class AddPopup : public geode::Popup<> {
public:
    TextInput* m_input_name;
    FunctionToolPopup* m_functool;

    static auto* create() {
        auto* node = new (std::nothrow) AddPopup();
        if (node && node->initAnchored(220, 130)) {
            node->autorelease();
        } else {
            delete node;
            node = nullptr;
        }
        return node;
    }

    bool setup() override {
        m_noElasticity = true;
        this->setTitle("Create new preset");

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = winSize / 2.f;

        auto const center2 = CCSize(220, 130) / 2;


        auto okSprite = ButtonSprite::create("Create", "bigFont.fnt", "GJ_button_01.png", .60f);
        okSprite->setScale(0.85f);

        auto okBtn = CCMenuItemSpriteExtra::create(
            okSprite,
            this,
            menu_selector(AddPopup::onCreate)
        );
        okBtn->setPosition(center2 + ccp(0, -39));
        m_buttonMenu->addChild(okBtn);

        m_input_name = TextInput::create(200, "", "chatFont.fnt");
        m_input_name->setLabel("Preset name");
        m_input_name->setPosition(center + ccp(0, 0));
        m_input_name->setCommonFilter(CommonFilter::Any);
        this->addChild(m_input_name);

        return true;
    }

    void onClose(CCObject* obj) override {
        Popup::onClose(obj);
    }

    void onCreate(CCObject* obj) {
        auto presets = Mod::get()->getSavedValue<std::vector<ToolConfig>>(
            "presets", std::vector<ToolConfig> {
                #include "default_presets.inc"
            }
        );

        presets.insert(presets.begin(), m_functool->saveConfig(m_input_name->getString()));

        Mod::get()->setSavedValue<std::vector<ToolConfig>>("presets", presets);

        this->onClose(nullptr);
    }
};
