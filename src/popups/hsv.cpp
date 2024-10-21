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

        auto okSprite = ButtonSprite::create("OK", "bigFont.fnt", "GJ_button_01.png", .60f);
        okSprite->setScale(0.85f);

        auto okBtn = CCMenuItemSpriteExtra::create(
            okSprite,
            this,
            menu_selector(HsvPopup::onClose)
        );

        okBtn->setPosition(center2 + ccp(0, -112));
        m_buttonMenu->addChild(okBtn);
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
