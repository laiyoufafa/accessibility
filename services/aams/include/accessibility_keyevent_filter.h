/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_KEYEVENT_FILTER_H_
#define OHOS_KEYEVENT_FILTER_H_

#include <map>
#include <vector>
#include <memory>

#include "accessibility_event_transmission.h"
#include "event_handler.h"

namespace OHOS {
namespace Accessibility {
class AccessibleAbilityConnection;
class AccessibleAbilityManagerService;
class KeyEventFilterEventHandler;

class KeyEventFilter : public EventTransmission {
public:
    struct ProcessingEvent {
        std::shared_ptr<MMI::KeyEvent> event_;
        uint32_t usedCount_;
        uint32_t seqNum_;
    };

    /**
     * @brief A constructor used to create a KeyEventFilter instance.
     * @param
     * @return
     */
    KeyEventFilter();

    /**
     * @brief A destructor used to delete the KeyEventFilter instance.
     * @param
     * @return
     */
    virtual ~KeyEventFilter();

    /**
     * @brief Handle key events from previous event stream node.
     * @param event the key event from Multimodal
     * @return
     */
    void OnKeyEvent(MMI::KeyEvent &event) override;

    /**
     * @brief Send key event to next stream node.
     * @param event the key event prepared to send
     * @return
     */
    void SendEventToParent(MMI::KeyEvent &event);

    /**
     * @brief Set AccessibleAbility keyevent result.
     * @param connection the AccessibleAbility
     * @param isHandled true if the AccessibleAbility can handle the event else false
     * @param sequenceNum the sequence of keyevent
     * @return
     */
    void SetServiceOnKeyEventResult(AccessibleAbilityConnection &connection, bool isHandled, uint32_t sequenceNum);

    /**
     * @brief Clear AccessibleAbility keyevents.
     * @param connection the AccessibleAbility
     * @return
     */
    void ClearServiceKeyEvents(AccessibleAbilityConnection &connection);

    /**
     * @brief Destroy the events.
     * @param
     * @return
     */
    void DestroyEvents() override;

    /**
     * @brief Remove the processing event.
     * @param event the event be removed
     * @return true if remove successfully else false
     */
    bool RemoveProcessingEvent(std::shared_ptr<ProcessingEvent> event);

private:
    /**
     * @brief Dispatch the keyevents.
     * @param event the keyevent from Multimodal
     * @return
     */
    void DispatchKeyEvent(MMI::KeyEvent &event);

    /**
     * @brief Find processing event.
     * @param connection the corresponding AccessiblityAbility
     * @param sequenceNum the squence of event
     * @return the processing event
     */
    std::shared_ptr<ProcessingEvent> FindProcessingEvent(AccessibleAbilityConnection &connection,
        uint32_t sequenceNum);

    std::shared_ptr<AccessibleAbilityManagerService> aams_ = nullptr;
    std::map<sptr<AccessibleAbilityConnection>, std::vector<std::shared_ptr<ProcessingEvent>>> eventMaps_;
    std::shared_ptr<KeyEventFilterEventHandler> timeouthandler_ = nullptr;
    std::shared_ptr<AppExecFwk::EventRunner> runner_ = nullptr;
    uint32_t sequenceNum_ = 0;
};

class KeyEventFilterEventHandler : public AppExecFwk::EventHandler {
public:
    /**
     * @brief A constructor used to create a KeyEventFilterEventHandler instance.
     * @param
     * @return
     */
    KeyEventFilterEventHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner, KeyEventFilter &keyEventFilter);
    virtual ~KeyEventFilterEventHandler() = default;

    /**
     * @brief Process the event of install system bundles.
     * @param event Indicates the event to be processed.
     * @return
     */
    virtual void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) override;

private:
    KeyEventFilter &keyEventFilter_;
};
} // namespace Accessibility
} // namespace OHOS
#endif  // OHOS_KEYEVENT_FILTER_H_