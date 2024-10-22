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


        auto okSprite = ButtonSprite::create("OK", "bigFont.fnt", "GJ_button_01.png", .60f);
        okSprite->setScale(0.85f);

        auto okBtn = CCMenuItemSpriteExtra::create(
            okSprite,
            this,
            menu_selector(ScalePopup::onClose)
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
