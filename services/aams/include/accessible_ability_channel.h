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

#ifndef ACCESSIBLE_ABILITY_CHANNEL_H
#define ACCESSIBLE_ABILITY_CHANNEL_H

#include <future>
#include "accessible_ability_channel_stub.h"
#include "event_handler.h"
#include "i_accessibility_element_operator.h"

namespace OHOS {
namespace Accessibility {
class AccessibleAbilityConnection;
class AccessibleAbilityChannel : public AccessibleAbilityChannelStub {
public:
    AccessibleAbilityChannel(const int32_t accountId, const std::string &clientName);
    ~AccessibleAbilityChannel() = default;
    bool SearchElementInfoByAccessibilityId(const int32_t accessibilityWindowId, const int32_t elementId,
        const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback,
        const int32_t mode) override;

    bool SearchElementInfosByText(const int32_t accessibilityWindowId, const int32_t elementId,
        const std::string &text, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    bool FindFocusedElementInfo(const int32_t accessibilityWindowId, const int32_t elementId,
        const int32_t focusType, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    bool FocusMoveSearch(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t direction,
        const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    bool ExecuteAction(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t action,
        const std::map<std::string, std::string> &actionArguments, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override;

    bool GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo) override;

    bool GetWindows(std::vector<AccessibilityWindowInfo> &windows) override;

    bool GetWindowsByDisplayId(const uint64_t displayId, std::vector<AccessibilityWindowInfo> &windows) override;

    bool ExecuteCommonAction(const int32_t action) override;

    void SetOnKeyPressEventResult(const bool handled, const int32_t sequence) override;

    bool SendSimulateGesture(const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath) override;

    bool SetEventTypeFilter(const uint32_t filter) override;

    bool SetTargetBundleName(const std::vector<std::string> &targetBundleNames) override;

private:
    static sptr<AccessibleAbilityConnection> GetConnection(int32_t accountId, const std::string &clientName);
    static sptr<IAccessibilityElementOperator> GetElementOperator(
        int32_t accountId, int32_t windowId, int32_t focusType, const std::string &clientName);
    bool GetWindows(uint64_t displayId, std::vector<AccessibilityWindowInfo> &windows) const;

    std::string clientName_ = "";
    int32_t accountId_ = -1;
    std::shared_ptr<AppExecFwk::EventHandler> eventHandler_ = nullptr;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBLE_ABILITY_CHANNEL_H