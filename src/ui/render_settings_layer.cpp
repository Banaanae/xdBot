#include "render_settings_layer.hpp"
#include "record_layer.hpp"

void RenderSettingsLayer::textChanged(CCTextInputNode* node) {

    if (secondsInput->getString() != "" && node == secondsInput) {
        std::string value = secondsInput->getString();
        if (value == ".")
            secondsInput->setString("0.");
        else if (std::count(value.begin(), value.end(), '.') == 2)
            return secondsInput->setString(mod->getSavedValue<std::string>("render_seconds_after").c_str());
    }

    std::string secondsAfter = secondsInput->getString();
    std::string args = argsInput->getString();
    std::string audioArgs = audioArgsInput->getString();
    std::string videoArgs = videoArgsInput->getString();
    std::string fadeIn = fadeInInput->getString();
    std::string fadeOut = fadeOutInput->getString();

    mod->setSavedValue("render_seconds_after", secondsAfter);
    mod->setSavedValue("render_args", args);
    mod->setSavedValue("render_audio_args", audioArgs);
    mod->setSavedValue("render_video_args", videoArgs);
    mod->setSavedValue("render_fade_in_time", fadeIn);
    mod->setSavedValue("render_fade_out_time", fadeOut);
}

void RenderSettingsLayer::onDefaults(CCObject*) {
	geode::createQuickPopup(
        "Restore",
        "<cr>Restore</c> default render settings?",
        "Cancel", "Yes",
        [this](auto, bool btn2) {
            auto& g = Global::get();
            
            g.mod->setSavedValue("render_args", std::string("-pix_fmt yuv420p"));
	        g.mod->setSavedValue("render_audio_args", std::string(""));
            g.mod->setSavedValue("render_video_args", std::string("colorspace=all=bt709:iall=bt470bg:fast=1"));
            g.mod->setSavedValue("render_record_audio", true);
            g.mod->setSavedValue("render_only_song", false);

	        CCArray* children = CCDirector::sharedDirector()->getRunningScene()->getChildren();
            CCObject* child;
            CCARRAY_FOREACH(children, child) {
                if (RecordLayer* layer = typeinfo_cast<RecordLayer*>(child)) {
                    layer->onClose(nullptr);
                    break;
                }
            }

            this->keyBackClicked();
            RecordLayer::openMenu(true);
            RenderSettingsLayer* layer = create();
            layer->m_noElasticity = true;
            layer->show();
        }
    );
}

bool RenderSettingsLayer::setup() {
    setTitle("Render Settings");

    cocos2d::CCPoint offset = (CCDirector::sharedDirector()->getWinSize() - m_mainLayer->getContentSize()) / 2;
    m_mainLayer->setPosition(m_mainLayer->getPosition() - offset);
    m_closeBtn->setPosition(m_closeBtn->getPosition() + offset);
    m_bgSprite->setPosition(m_bgSprite->getPosition() + offset);
    m_title->setPosition(m_title->getPosition() + offset);
    
    mod = Mod::get();
    
    Utils::setBackgroundColor(m_bgSprite);

    if (mod->getSavedValue<std::string>("render_seconds_after") == "")
        mod->setSavedValue("render_seconds_after", std::to_string(0));

    cocos2d::CCSize center = cocos2d::CCDirector::sharedDirector()->getWinSize() / 2;
    CCSprite* spriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    CCSprite* spriteOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

    CCMenu* menu = CCMenu::create();
    m_mainLayer->addChild(menu);
    menu->setPositionX(menu->getPositionX() - 67);

    CCScale9Sprite* bg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    bg->setScale(0.355f);
    bg->setColor({ 0,0,0 });
    bg->setOpacity(75);
    bg->setPosition(ccp(-28, 97));
    bg->setAnchorPoint({ 0, 1 });
    bg->setContentSize({ 392, 55 });
    menu->addChild(bg);

    CCLabelBMFont* lbl = CCLabelBMFont::create("Extra Args:", "bigFont.fnt");
    lbl->setPosition(ccp(-105, 88));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    argsInput = CCTextInputNode::create(150, 30, "args", "chatFont.fnt");
    argsInput->m_textField->setAnchorPoint({ 0.5f, 0.5f });
    argsInput->ignoreAnchorPointForPosition(true);
    argsInput->m_placeholderLabel->setAnchorPoint({ 0.5f, 0.5f });
    argsInput->setPosition(ccp(25, 86));
    argsInput->setLabelPlaceholderColor(ccc3(163, 135, 121));
    argsInput->setMouseEnabled(true);
    argsInput->setTouchEnabled(true);
    argsInput->setContentSize({ 120, 20 });
    argsInput->setMaxLabelWidth(170.f);
    argsInput->setScale(0.75);
    argsInput->setString(mod->getSavedValue<std::string>("render_args").c_str());
    argsInput->setDelegate(this);
    argsInput->setAllowedChars(" 0123456789abcdefghijklmnopqrstuvwxyz-_.\"\\/");
    menu->addChild(argsInput);

    bg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    bg->setScale(0.355f);
    bg->setColor({ 0,0,0 });
    bg->setOpacity(75);
    bg->setPosition(ccp(-31, 65));
    bg->setAnchorPoint({ 0, 1 });
    bg->setContentSize({ 401, 55 });
    menu->addChild(bg);

    lbl = CCLabelBMFont::create("Audio Args:", "bigFont.fnt");
    lbl->setPosition(ccp(-105, 55));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    audioArgsInput = CCTextInputNode::create(150, 30, "audio args", "chatFont.fnt");
    audioArgsInput->m_textField->setAnchorPoint({ 0.5f, 0.5f });
    audioArgsInput->ignoreAnchorPointForPosition(true);
    audioArgsInput->m_placeholderLabel->setAnchorPoint({ 0.5f, 0.5f });
    audioArgsInput->setPosition(ccp(18, 53));
    audioArgsInput->setLabelPlaceholderColor(ccc3(163, 135, 121));
    audioArgsInput->setMouseEnabled(true);
    audioArgsInput->setTouchEnabled(true);
    audioArgsInput->setContentSize({ 180, 20 });
    audioArgsInput->setMaxLabelWidth(165.f);
    audioArgsInput->setScale(0.75);
    audioArgsInput->setString(mod->getSavedValue<std::string>("render_audio_args").c_str());
    audioArgsInput->setDelegate(this);
    audioArgsInput->setAllowedChars(" 0123456789abcdefghijklmnopqrstuvwxyz-_.\"\\/");
    menu->addChild(audioArgsInput);

    bg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    bg->setScale(0.375f);
    bg->setColor({ 0,0,0 });
    bg->setOpacity(75);
    bg->setPosition(ccp(49, 4));
    bg->setAnchorPoint({ 0, 1 });
    bg->setContentSize({ 82, 55 });
    menu->addChild(bg);

    bg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    bg->setScale(0.355f);
    bg->setColor({ 0,0,0 });
    bg->setOpacity(75);
    bg->setPosition({-29, 34});
    bg->setAnchorPoint({ 0, 1 });
    bg->setContentSize({ 395, 55 });
    menu->addChild(bg);

    videoArgsInput = CCTextInputNode::create(150, 30, "video args", "chatFont.fnt");
    videoArgsInput->m_textField->setAnchorPoint({ 0.5f, 0.5f });
    videoArgsInput->ignoreAnchorPointForPosition(true);
    videoArgsInput->m_placeholderLabel->setAnchorPoint({ 0.5f, 0.5f });
    videoArgsInput->setPosition({19, 22});
    videoArgsInput->setLabelPlaceholderColor(ccc3(163, 135, 121));
    videoArgsInput->setMouseEnabled(true);
    videoArgsInput->setTouchEnabled(true);
    videoArgsInput->setContentSize({ 180, 20 });
    videoArgsInput->setMaxLabelWidth(165.f);
    videoArgsInput->setScale(0.75);
    videoArgsInput->setString(mod->getSavedValue<std::string>("render_video_args").c_str());
    videoArgsInput->setDelegate(this);
    menu->addChild(videoArgsInput);

    lbl = CCLabelBMFont::create("Video Args:", "bigFont.fnt");
    lbl->setAnchorPoint({0, 0.5});
    lbl->setOpacity(200);
    lbl->setScale(0.325f);
    lbl->setPosition({-105, 24});
    menu->addChild(lbl);

    lbl = CCLabelBMFont::create("Render after completion:", "bigFont.fnt");
    lbl->setPosition(ccp(-105, -5));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    secondsInput = CCTextInputNode::create(150, 30, "sec", "chatFont.fnt");
    secondsInput->m_textField->setAnchorPoint({ 0.5f, 0.5f });
    secondsInput->ignoreAnchorPointForPosition(true);
    secondsInput->m_placeholderLabel->setAnchorPoint({ 0.5f, 0.5f });
    secondsInput->setPosition(ccp(50, -8));
    secondsInput->setLabelPlaceholderColor(ccc3(163, 135, 121));
    secondsInput->setMouseEnabled(true);
    secondsInput->setTouchEnabled(true);
    secondsInput->setContentSize({ 120, 20 });
    secondsInput->setMaxLabelWidth(90.f);
    secondsInput->setScale(0.75);
    secondsInput->setString(mod->getSavedValue<std::string>("render_seconds_after").c_str());
    secondsInput->setDelegate(this);
    secondsInput->setMaxLabelLength(2);
    secondsInput->setAllowedChars("0123456789.");
    menu->addChild(secondsInput);

    lbl = CCLabelBMFont::create("s", "chatFont.fnt");
    lbl->setPosition(ccp(84, -5));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setScale(0.825);
    menu->addChild(lbl);

    lbl = CCLabelBMFont::create("Only Add Song:", "bigFont.fnt");
    lbl->setPosition(ccp(-105, -32));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    onlySongToggle = CCMenuItemToggler::create(spriteOff, spriteOn, this, menu_selector(RecordLayer::toggleSetting));
    onlySongToggle->setPosition(ccp(0, -32));
    onlySongToggle->setScale(0.555);
    onlySongToggle->toggle(mod->getSavedValue<bool>("render_only_song"));
    onlySongToggle->setID("render_only_song");
    menu->addChild(onlySongToggle);

    lbl = CCLabelBMFont::create("Record Audio:", "bigFont.fnt");
    lbl->setPosition(ccp(-105, -58));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    recordAudioToggle = CCMenuItemToggler::create(spriteOff, spriteOn, this, menu_selector(RecordLayer::toggleSetting));
    recordAudioToggle->setPosition(ccp(0, -58));
    recordAudioToggle->setScale(0.555);
    recordAudioToggle->toggle(mod->getSavedValue<bool>("render_record_audio"));
    recordAudioToggle->setID("render_record_audio");
    menu->addChild(recordAudioToggle);

    lbl = CCLabelBMFont::create("Fade In:", "bigFont.fnt");
    lbl->setPosition(ccp(25, -32));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    fadeInInput = TextInput::create(50.f, "s", "bigFont.fnt");
    fadeInInput->setScale(0.5f);
    fadeInInput->setPosition(ccp(100, -32));
    fadeInInput->setString(Mod::get()->getSavedValue<std::string>("render_fade_in_time").c_str());
    fadeInInput->getInputNode()->setDelegate(this);
    menu->addChild(fadeInInput);

    CCMenuItemToggler* toggle = CCMenuItemToggler::create(spriteOff, spriteOn, this, menu_selector(RecordLayer::toggleSetting));
    toggle->setPosition(ccp(130, -32));
    toggle->setScale(0.555);
    toggle->toggle(mod->getSavedValue<bool>("render_fade_in"));
    toggle->setID("render_fade_in");
    menu->addChild(toggle);

    lbl = CCLabelBMFont::create("Fade Out:", "bigFont.fnt");
    lbl->setPosition(ccp(25, -58));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    fadeOutInput = TextInput::create(50.f, "s", "bigFont.fnt");
    fadeOutInput->setScale(0.5f);
    fadeOutInput->setPosition(ccp(100, -58));
    fadeOutInput->setString(Mod::get()->getSavedValue<std::string>("render_fade_out_time").c_str());
    fadeOutInput->getInputNode()->setDelegate(this);
    menu->addChild(fadeOutInput);

    toggle = CCMenuItemToggler::create(spriteOff, spriteOn, this, menu_selector(RecordLayer::toggleSetting));
    toggle->setPosition(ccp(130, -58));
    toggle->setScale(0.555);
    toggle->toggle(mod->getSavedValue<bool>("render_fade_out"));
    toggle->setID("render_fade_out");
    menu->addChild(toggle);

    lbl = CCLabelBMFont::create("Hide Endscreen:", "bigFont.fnt");
    lbl->setPosition(ccp(-105, -83));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.325);
    menu->addChild(lbl);

    toggle = CCMenuItemToggler::create(spriteOff, spriteOn, this, menu_selector(RecordLayer::toggleSetting));
    toggle->setPosition(ccp(0, -83));
    toggle->setScale(0.555);
    toggle->toggle(mod->getSavedValue<bool>("render_hide_endscreen"));
    toggle->setID("render_hide_endscreen");
    menu->addChild(toggle);

    lbl = CCLabelBMFont::create("Hide Level Complete:", "bigFont.fnt");
    lbl->setPosition(ccp(-105, -108));
    lbl->setAnchorPoint({ 0, 0.5 });
    lbl->setOpacity(200);
    lbl->setScale(0.25);
    menu->addChild(lbl);

    toggle = CCMenuItemToggler::create(spriteOff, spriteOn, this, menu_selector(RecordLayer::toggleSetting));
    toggle->setPosition(ccp(0, -108));
    toggle->setScale(0.555);
    toggle->toggle(mod->getSavedValue<bool>("render_hide_levelcomplete"));
    toggle->setID("render_hide_levelcomplete");
    menu->addChild(toggle);

    lbl = CCLabelBMFont::create("SFX Volume", "goldFont.fnt");
    lbl->setScale(0.475f);
    lbl->setPosition({188, 87});
    menu->addChild(lbl);

    sfxSlider = Slider::create(
		this,
		menu_selector(RenderSettingsLayer::onSlider),
		1.f
	);
	sfxSlider->setPosition({188, 69});
	sfxSlider->setAnchorPoint({ 0.f, 0.f });
	sfxSlider->setScale(0.545f);
	sfxSlider->setValue(Mod::get()->getSavedValue<double>("render_sfx_volume"));
	menu->addChild(sfxSlider);

    lbl = CCLabelBMFont::create("Music Volume", "goldFont.fnt");
    lbl->setScale(0.475f);
    lbl->setPosition({188, 42});
    menu->addChild(lbl);

    musicSlider = Slider::create(
		this,
		menu_selector(RenderSettingsLayer::onSlider),
		1.f
	);
	musicSlider->setPosition({188, 24});
	musicSlider->setAnchorPoint({ 0.f, 0.f });
	musicSlider->setScale(0.545f);
	musicSlider->setValue(Mod::get()->getSavedValue<double>("render_music_volume"));
	menu->addChild(musicSlider);

    ButtonSprite* spr = ButtonSprite::create("Ok");
    spr->setScale(0.875);
    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(RenderSettingsLayer::close));
    btn->setPosition(ccp(67, -100));
    menu->addChild(btn);

    spr = ButtonSprite::create("Restore Defaults");
    spr->setScale(0.375f);
    btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(RenderSettingsLayer::onDefaults));
    btn->setPosition({200, -123});
    menu->addChild(btn);

    return true;
}

void RenderSettingsLayer::onSlider(CCObject*) {
    Mod::get()->setSavedValue("render_sfx_volume", sfxSlider->getValue());
    Mod::get()->setSavedValue("render_music_volume", musicSlider->getValue());
}