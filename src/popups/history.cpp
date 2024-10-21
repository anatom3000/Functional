class HistoryPopup : public geode::Popup<> {
public:
    FunctionToolPopup* m_functool;

    static auto* create(FunctionToolPopup* functool) {
        auto* node = new (std::nothrow) HistoryPopup();
        node->m_functool = functool;
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
        this->setTitle("Presets");

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = winSize / 2.f;

        auto const center2 = CCSize(440, 280) / 2;

        auto entries = CCArray::create();
        entries->retain();
        for (int i = 0; i < 5; i++) {
            entries->addObject(ConfigCell::create({
                .name = "uwu rawr",
                .x = "42.0",
                .y = "t",
                .n = "15",
                .start = "0.0",
                .end = "5.0",
                .abs_scaling = true,
                .abs_rotation = false,
                .use_robtop_units = true,
                .use_radians = false,
                .delete_original = true,
                .scale_x = "2.0",
                .scale_y = "0.5",
                .base_hue = "t",
            }, m_functool));
            entries->addObject(ConfigCell::create({
                .name = "aaa :x",
                .x = "42.0",
                .y = "t",
                .n = "15",
                .start = "0.0",
                .end = "5.0",
                .abs_scaling = true,
                .abs_rotation = false,
                .use_robtop_units = true,
                .use_radians = false,
                .delete_original = true,
                .scale_x = "2.0",
                .scale_y = "0.5",
                .base_hue = "t",
            }, m_functool));
        }

        auto listView = geode::ListView::create(entries, 60, 400, 190);
        listView->setPosition(center2 - ccp(200, 85));
        m_buttonMenu->addChild(listView);

        auto okSprite = ButtonSprite::create("OK", "bigFont.fnt", "GJ_button_01.png", .60f);
        okSprite->setScale(0.85f);

        auto okBtn = CCMenuItemSpriteExtra::create(
            okSprite,
            this,
            menu_selector(HsvPopup::onClose)
        );
        okBtn->setPosition(center2 + ccp(0, -112));
        m_buttonMenu->addChild(okBtn);

        return true;
    }

    void onClose(CCObject* obj) override {
        // save stuff
        Popup::onClose(obj);
    }
};


class ConfigCell : public cocos2d::CCLayer {
public:
    ToolConfig m_config;
    FunctionToolPopup* m_functool;

    static ConfigCell* create(ToolConfig c, FunctionToolPopup* functool) {
        ConfigCell* node = new ConfigCell();
        if (node && node->init(c, functool)) {
            node->autorelease();
        } else {
            delete node;
            node = nullptr;
        }
        return node;
    }

    bool init(ToolConfig c, FunctionToolPopup* functool) {
        if (!CCLayer::init()) return false;

        m_functool = functool;

        auto center = ccp(400, 60) / 2.f;

        m_config = c;
        auto name = CCLabelBMFont::create(c.name.c_str(), "bigFont.fnt");
        name->setScale(0.5f);
        name->setPosition(center + ccp(-195+name->getScaledContentSize().width/2.0, 22));
        this->addChild(name);

        auto toggles_content = std::format(
            "{}{}{}{}{}",
            c.abs_scaling ?      "absolute scaling, "  : "",
            c.abs_rotation ?     "absolute rotation, " : "",
            c.use_robtop_units ? "use robtop units, "  : "",
            c.use_radians ?      "use radians, "       : "",
            c.delete_original ?  "delete original, "   : ""
        );

        if (toggles_content.length() != 0) {
            toggles_content.pop_back();
            toggles_content.pop_back();
            toggles_content.push_back('.');

            toggles_content[0] = static_cast<char>(
                std::toupper(static_cast<unsigned char>(toggles_content[0]))
            );
        }
        
        auto infos_content = std::format(
            "Position(t) = ({}, {}) ; Rotation(t) = {} ; Scale(t) = ({}, {})\n"
            "Amount: {} ; Start t: {} ; End t: {}\n"
            "HSV Base: ({}, {}, {}) ; Detail: ({}, {}, {})\n"
            "{}",
            c.x.length() != 0 ? c.x : "N/A", 
            c.y.length() != 0 ? c.y : "N/A", 
            c.rotation.length() != 0 ? c.rotation : "N/A",
            c.scale_x.length() != 0 ? c.scale_x : "N/A",
            c.scale_y.length() != 0 ? c.scale_y : "N/A",

            c.n.length() != 0 ? c.n : "N/A", 
            c.start.length() != 0 ? c.start : "N/A", 
            c.end.length() != 0 ? c.end : "N/A",

            c.base_hue.length() != 0 ? c.base_hue : "N/A", 
            c.base_saturation.length() != 0 ? c.base_saturation : "N/A", 
            c.base_value.length() != 0 ? c.base_value : "N/A", 
            c.detail_hue.length() != 0 ? c.detail_hue : "N/A", 
            c.detail_saturation.length() != 0 ? c.detail_saturation : "N/A", 
            c.detail_value.length() != 0 ? c.detail_value : "N/A",

            toggles_content
        );
        auto infos = CCLabelBMFont::create(infos_content.c_str(), "chatFont.fnt");
        infos->setScale(0.6f);
        infos->setPosition(center + ccp(-194+infos->getScaledContentSize().width/2.0, -6));
        this->addChild(infos);

        auto button_menu = CCMenu::create();

        auto useSprite = CCSprite::createWithSpriteFrameName("GJ_selectSongBtn_001.png");
        useSprite->setScale(0.7f);

        auto useBtn = CCMenuItemSpriteExtra::create(
            useSprite,
            this,
            menu_selector(ConfigCell::onUse)
        );

        useBtn->setPosition(center + ccp(180, 0));
        button_menu->addChild(useBtn);

        button_menu->setPosition(ccp(0, 0));
        this->addChild(button_menu);

        return true;
    }

    void onUse(CCObject*) {
        m_functool->loadConfig(m_config);
    }
};
