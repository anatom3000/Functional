
class HistoryPopup : public geode::Popup<> {
public:
    FunctionToolPopup* m_functool;
    std::vector<ToolConfig> m_presets;
    geode::ScrollLayer* m_scrollLayer;

    static HistoryPopup* create(FunctionToolPopup* functool) {
        auto* ret = new (std::nothrow) HistoryPopup();
        ret->m_functool = functool;
        if (ret && ret->initAnchored(440, 280)) {
            ret->autorelease();
        } else {
            delete ret;
            ret = nullptr;
        }
        return ret;
    }

    bool setup() override {
        m_noElasticity = true;
        this->setTitle("Presets");

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = winSize / 2.f;

        auto const center2 = CCSize(440, 280) / 2;

        m_presets = Mod::get()->getSavedValue<std::vector<ToolConfig>>(
            "presets", std::vector<ToolConfig> {
            ToolConfig {
                .name = "Circle",
                .x = "4*sin(t)",
                .y = "4*cos(t)",
                .rotation = "t",
                .n = "16",
                .start = "0",
                .end = "360",
                .delete_original = true
            }
        });
        
        // inspired by https://github.com/hiimjustin000/SearchHistory/blob/v1.1.0/src/SearchHistoryPopup.cpp
        m_scrollLayer = ScrollLayer::create({ 400.0f, 190.0f });
        m_scrollLayer->setPosition(20.0f, 50.0f);
        m_scrollLayer->m_contentLayer->setLayout(
            ColumnLayout::create()
                ->setAxisReverse(true)
                ->setAxisAlignment(AxisAlignment::End)
                ->setAutoGrowAxis(195.0f)
                ->setGap(0.0f)
        );
        m_mainLayer->addChild(m_scrollLayer);

        this->refreshEntries(true);

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

    void refreshEntries(bool reset) {
        m_scrollLayer->m_contentLayer->removeAllChildren();

        int i = 0;
        for (auto pre : m_presets) {
            auto cell = ConfigCell::create(pre, m_functool, i, this->m_presets.size());

            cell->m_history = this;
            m_scrollLayer->m_contentLayer->addChild(cell);

            i++;
        }

        m_scrollLayer->m_contentLayer->updateLayout();
        if (reset) m_scrollLayer->scrollToTop();
    }

    void onClose(CCObject* obj) override {
        Mod::get()->setSavedValue<std::vector<ToolConfig>>("presets", m_presets);

        Popup::onClose(obj);
    }
};

class ConfigCell : public cocos2d::CCLayerColor {
public:
    ToolConfig m_config;
    FunctionToolPopup* m_functool;
    HistoryPopup* m_history;
    int m_index;
    int m_count;

    static ConfigCell* create(ToolConfig c, FunctionToolPopup* functool, int index, int count) {
        ConfigCell* node = new ConfigCell();
        if (node && node->init(c, functool, index, count)) {
            node->autorelease();
        } else {
            delete node;
            node = nullptr;
        }
        return node;
    }

    void draw() override {
        CCLayerColor::draw();

        ccDrawColor4B(0, 0, 0, 75);
        glLineWidth(2.0f);
        if (m_index < m_count - 1) ccDrawLine({ 0.0f,  0.0f }, { 400.0f,  0.0f });
        if (m_index > 0)           ccDrawLine({ 0.0f, 60.0f }, { 400.0f, 60.0f });
    }

    bool init(ToolConfig c, FunctionToolPopup* functool, int index, int count) {
        if (!CCLayerColor::init()) return false;

        m_functool = functool;
        m_config = c;
        m_index = index;
        m_count = count;

        this->setContentSize({400.f, 60.f});
        this->setOpacity(index % 2 == 0 ? 50 : 100);


        auto center = ccp(400, 60) / 2.f;

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
        infos->setPosition(center + ccp(-193+infos->getScaledContentSize().width/2.0, -6));
        this->addChild(infos);

        auto button_menu = CCMenu::create();

        auto useSprite = CCSprite::createWithSpriteFrameName("GJ_selectSongBtn_001.png");
        auto deleteSprite = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
        useSprite->setScale(0.70f);
        deleteSprite->setScale(0.63f);

        auto useBtn = CCMenuItemSpriteExtra::create(
            useSprite,
            this,
            menu_selector(ConfigCell::onUse)
        );

        auto deleteBtn = CCMenuItemSpriteExtra::create(
            deleteSprite,
            this,
            menu_selector(ConfigCell::onDelete)
        );


        useBtn->setPosition(center + ccp(180, 15));
        deleteBtn->setPosition(center + ccp(180, -15));
        button_menu->addChild(useBtn);
        button_menu->addChild(deleteBtn);

        button_menu->setPosition(ccp(0, 0));
        this->addChild(button_menu);

        return true;
    }

    void onUse(CCObject*) {
        m_functool->loadConfig(m_config);
        m_history->onClose(nullptr);
    }

    void onDelete(CCObject*) {
        m_history->m_presets.erase(m_history->m_presets.begin() + m_index);
        m_history->refreshEntries(m_index == 0);
    }
};
