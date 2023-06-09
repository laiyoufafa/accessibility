/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef MOCK_ACCESSIBLE_ABILITY_CONNECTION_H_
#define MOCK_ACCESSIBLE_ABILITY_CONNECTION_H_

#include <gmock/gmock.h>
#include "accessible_ability_connection.h"

namespace OHOS {
namespace Accessibility {
class MockAccessibleAbilityChannel : public AccessibleAbilityChannel {
public:
    MockAccessibleAbilityChannel(AccessibleAbilityConnection& connection);
    ~MockAccessibleAbilityChannel();

    MOCK_METHOD5(SearchElementInfoByAccessibilityId,
        bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t requestId,
            const sptr<IAccessibilityElementOperatorCallback>& callback, const int32_t mode));

    MOCK_METHOD5(SearchElementInfosByText,
        bool(const int32_t accessibilityWindowId, const int32_t elementId, const std::string& text,
            const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD5(FindFocusedElementInfo,
        bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t focusType,
            const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));

    MOCK_METHOD5(
        FocusMoveSearch, bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t direction,
                             const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD6(ExecuteAction, bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t action,
                                    std::map<std::string, std::string>& actionArguments, const int32_t requestId,
                                    const sptr<IAccessibilityElementOperatorCallback>& callback));

    MOCK_METHOD0(GetWindows, std::vector<AccessibilityWindowInfo>());
    MOCK_METHOD1(ExecuteCommonAction, bool(const int32_t action));
    MOCK_METHOD2(SetOnKeyPressEventResult, void(const bool handled, const int32_t sequence));
    MOCK_METHOD1(GetDisplayResizeScale, float(const int32_t displayId));
    MOCK_METHOD1(GetDisplayResizeCenterX, float(const int32_t displayId));

    MOCK_METHOD1(GetDisplayResizeCenterY, float(const int32_t displayId));
    MOCK_METHOD1(GetDisplayResizeRect, Rect(const int32_t displayId));
    MOCK_METHOD2(ResetDisplayResize, bool(const int32_t displayId, bool animate));

    MOCK_METHOD5(SetDisplayResizeScaleAndCenter,
        bool(const int32_t displayId, const float scale, const float centerX, const float centerY, const bool animate));
    MOCK_METHOD2(
        SendSimulateGesture, void(const int32_t requestId, const std::vector<AccessibilityGesturePath>& gestureSteps));
};

class MockAccessibleAbilityConnection : public AccessibleAbilityConnection {
public:
    MockAccessibleAbilityConnection(const sptr<AccessibilityAccountData>& accountData, const int32_t connectionId,
        AccessibilityAbilityInfo& abilityInfo);

    virtual ~MockAccessibleAbilityConnection();

    MOCK_METHOD3(OnAbilityConnectDone,
        void(const AppExecFwk::ElementName& element, const sptr<IRemoteObject>& remoteObject, int32_t resultCode));
    MOCK_METHOD2(OnAbilityDisconnectDone, void(const AppExecFwk::ElementName& element, int32_t resultCode));
    // For AccessibleAbilityClientProxy
    MOCK_METHOD1(OnAccessibilityEvent, void(AccessibilityEventInfo& eventInfo));
    MOCK_METHOD2(OnKeyPressEvent, bool(const MMI::KeyEvent& keyEvent, const int32_t sequence));
    MOCK_METHOD5(OnDisplayResized,
        void(const int32_t displayId, const Rect& rect, const float scale, const float centerX, const float centerY));
    MOCK_METHOD2(OnGestureSimulateResult, bool(const int32_t sequence, const bool completedSuccessfully));
    // Get Attribution
    AccessibilityAbilityInfo abilityInfo_;
    AppExecFwk::ElementName elementName_;
    inline AccessibilityAbilityInfo& GetAbilityInfo()
    {
        return abilityInfo_;
    }

    inline AppExecFwk::ElementName& GetElementName()
    {
        return elementName_;
    }

    inline wptr<AccessibilityAccountData> GetAccountData()
    {
        return nullptr;
    }

    inline sptr<IAccessibleAbilityClient> GetProxy()
    {
        return nullptr;
    }

    MOCK_METHOD0(Disconnect, void());
    MOCK_METHOD1(Connect, void(const AppExecFwk::ElementName& element));
    MOCK_METHOD0(GetChannelId, int32_t());
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_ACCESSIBLE_ABILITY_CONNECTION_H_