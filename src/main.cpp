#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

static const std::string DISCORD_URL =
    "https://discord.com/channels/398627612299362304/427879275434213387";

class $modify(SendToModeratorsLayer, LevelInfoLayer) {
    void onSendToModerators(CCObject*) {
        geode::utils::web::openLinkInBrowser(DISCORD_URL);
    }

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        // m_stars == 0 means unrated
        // m_featured == 0 means not featured
        // m_isEpic == 0 means not epic (1), legendary (2), or mythic (3)
        bool isUnrated = level->m_stars == 0
            && level->m_featured == 0
            && level->m_isEpic == 0;

        if (isUnrated) {
            auto sprite = ButtonSprite::create(
                "Send", "goldFont.fnt", "GJ_button_01.png", 0.8f
            );
            auto btn = CCMenuItemSpriteExtra::create(
                sprite, this,
                menu_selector(SendToModeratorsLayer::onSendToModerators)
            );

            // Try to attach to the existing left-side-menu so it fits naturally
            if (auto menu = getChildByID("left-side-menu")) {
                menu->addChild(btn);
                static_cast<CCNode*>(menu)->updateLayout();
            } else {
                // Fallback: create our own menu on the left side
                auto fallbackMenu = CCMenu::create();
                fallbackMenu->setID("send-to-mods-menu"_spr);
                fallbackMenu->setPosition({ 50.f, 200.f });
                fallbackMenu->addChild(btn);
                addChild(fallbackMenu, 10);
            }
        }

        return true;
    }
};
