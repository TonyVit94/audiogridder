/*
 * Copyright (c) 2020 Andreas Pohl
 * Licensed under MIT (https://github.com/apohl79/audiogridder/blob/master/COPYING)
 *
 * Author: Andreas Pohl
 */

#include "ServerSettingsWindow.hpp"
#include "App.hpp"

namespace e47 {

ServerSettingsWindow::ServerSettingsWindow(App* app)
    : DocumentWindow("Server Settings",
                     LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
                     DocumentWindow::closeButton),
      m_app(app) {
    setUsingNativeTitleBar(true);

    auto label = std::make_unique<Label>();
    label->setText("Server ID:", NotificationType::dontSendNotification);
    label->setBounds(15, 40, 250, 30);
    addChildAndSetID(label.get(), "lbl");
    m_components.push_back(std::move(label));

    String id;
    id << m_app->getServer().getId();
    m_idText.setText(id);
    m_idText.setBounds(280, 43, 50, 25);
    addChildAndSetID(&m_idText, "id");

    label = std::make_unique<Label>();
    label->setText("AudioUnit Support:", NotificationType::dontSendNotification);
    label->setBounds(15, 80, 250, 30);
#ifdef JUCE_WINDOWS
    label->setAlpha(0.5);
#endif
    addChildAndSetID(label.get(), "lbl");
    m_components.push_back(std::move(label));

    m_auSupport.setBounds(307, 83, 25, 25);
    m_auSupport.setToggleState(m_app->getServer().getEnableAU(), NotificationType::dontSendNotification);
#ifdef JUCE_WINDOWS
    m_auSupport.setToggleState(false, NotificationType::dontSendNotification);
    m_auSupport.setAlpha(0.5);
    m_auSupport.setEnabled(false);
#endif
    addChildAndSetID(&m_auSupport, "au");

    label = std::make_unique<Label>();
    label->setText("VST3 Support:", NotificationType::dontSendNotification);
    label->setBounds(15, 120, 250, 30);
    addChildAndSetID(label.get(), "lbl");
    m_components.push_back(std::move(label));

    m_vstSupport.setBounds(307, 123, 25, 25);
    m_vstSupport.setToggleState(m_app->getServer().getEnableVST(), NotificationType::dontSendNotification);
    addChildAndSetID(&m_vstSupport, "vst");

    label = std::make_unique<Label>();
    label->setText("Screen Capture Diff Detection:", NotificationType::dontSendNotification);
    label->setBounds(15, 160, 250, 30);
    addChildAndSetID(label.get(), "lbl");
    m_components.push_back(std::move(label));

    m_screenDiffDetection.setBounds(307, 163, 25, 25);
    m_screenDiffDetection.setToggleState(m_app->getServer().getScreenDiffDetection(),
                                         NotificationType::dontSendNotification);
    m_screenDiffDetection.onClick = [this] {
        if (m_screenDiffDetection.getToggleState()) {
            m_screenJpgQualityLbl.setAlpha(0.5);
            m_screenJpgQuality.setEnabled(false);
            m_screenJpgQuality.setAlpha(0.5);
        } else {
            m_screenJpgQualityLbl.setAlpha(1);
            m_screenJpgQuality.setEnabled(true);
            m_screenJpgQuality.setAlpha(1);
        }
    };
    m_screenDiffDetection.onClick();
    addChildAndSetID(&m_screenDiffDetection, "diff");

    m_screenJpgQualityLbl.setText("Screen Capture Quality (0.1-1.0):", NotificationType::dontSendNotification);
    m_screenJpgQualityLbl.setBounds(15, 200, 250, 30);
    addChildAndSetID(&m_screenJpgQualityLbl, "lbl");

    String q;
    q << m_app->getServer().getScreenQuality();
    m_screenJpgQuality.setText(q);
    m_screenJpgQuality.setBounds(280, 203, 50, 25);
    addChildAndSetID(&m_screenJpgQuality, "qual");

    m_saveButton.setButtonText("Save");
    m_saveButton.setBounds(112, 250, 125, 30);
    m_saveButton.onClick = [this, app] {
        auto appCpy = app;
        appCpy->getServer().setId(m_idText.getTextValue().toString().getIntValue());
        appCpy->getServer().setEnableAU(m_auSupport.getToggleState());
        appCpy->getServer().setEnableVST(m_vstSupport.getToggleState());
        appCpy->getServer().setScreenDiffDetection(m_screenDiffDetection.getToggleState());
        float qual = m_screenJpgQuality.getTextValue().toString().getFloatValue();
        if (qual < 0.1) {
            qual = 0.1f;
        } else if (qual > 1) {
            qual = 1.0f;
        }
        appCpy->getServer().setScreenQuality(qual);
        appCpy->getServer().saveConfig();
        appCpy->hideServerSettings();
        appCpy->restartServer();
    };
    addChildAndSetID(&m_saveButton, "save");

    setResizable(false, false);
    centreWithSize(350, 300);
    setVisible(true);
}

void ServerSettingsWindow::closeButtonPressed() { m_app->hideServerSettings(); }

}  // namespace e47
