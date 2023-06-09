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

#ifndef MOCK_ACCESSIBLE_ABILITY_MANAGER_SERVICE_H
#define MOCK_ACCESSIBLE_ABILITY_MANAGER_SERVICE_H

#include <gmock/gmock.h>
#include "accessible_ability_manager_service.h"

namespace OHOS {
namespace Accessibility {
class MockAccessibleAbilityManagerService : public AccessibleAbilityManagerService {
    DECLARE_DELAYED_SINGLETON(MockAccessibleAbilityManagerService)
    DECLEAR_SYSTEM_ABILITY(MockAccessibleAbilityManagerService)
public:
    MOCK_METHOD0(OnStart, void());
    MOCK_METHOD0(OnStop, void());
    MOCK_METHOD2(SendEvent, void(const AccessibilityEventInfo& uiEvent, const int32_t accountId));
    MOCK_METHOD2(RegisterStateCallback,
        uint32_t(const sptr<AccessibleAbilityManagerServiceStub>& callback, const int32_t accountId));
    MOCK_METHOD2(RegisterCaptionPropertyCallback,
        uint32_t(const sptr<IAccessibleAbilityManagerCaptionObserver>& callback, const int32_t accountId));
    MOCK_METHOD2(
        GetAbilityList, std::vector<AccessibilityAbilityInfo>(const uint32_t abilityTypes, const int32_t stateType));
    MOCK_METHOD3(RegisterElementOperator,
        void(const int32_t windowId, const sptr<IAccessibilityElementOperator>& operation, const int32_t accountId));
    MOCK_METHOD1(DeregisterElementOperator, void(const int32_t windowId));
    MOCK_METHOD0(GetCaptionProperty, CaptionProperty());
    MOCK_METHOD1(SetCaptionProperty, bool(const CaptionProperty& caption));
    MOCK_METHOD1(SetCaptionState, bool(const bool state));
    MOCK_METHOD1(SetEnabled, bool(const bool state));
    MOCK_METHOD1(SetTouchEventInjector, void(const sptr<TouchEventInjector>& touchEventInjector));

    inline sptr<TouchEventInjector> GetTouchEventInjector()
    {
        return nullptr;
    }

    MOCK_METHOD1(IsWantedKeyEvent, bool(MMI::KeyEvent& event));

    inline sptr<KeyEventFilter> GetKeyEventFilter()
    {
        return nullptr;
    }

    MOCK_METHOD1(SetKeyEventFilter, void(const sptr<KeyEventFilter>& keyEventFilter));
    MOCK_METHOD5(NotifyDisplayResizeStateChanged,
        void(int32_t displayId, Rect& rect, float scale, float centerX, float centerY));

    inline int32_t GetCurrentAccountId()
    {
        return 0;
    }

    inline uint32_t GetConnectCounter()
    {
        return 0;
    }

    std::shared_ptr<AppExecFwk::EventRunner> runner_;
    std::shared_ptr<AAMSEventHandler> handler_;
    inline std::shared_ptr<AAMSEventHandler>& GetMainHandler()
    {
        return handler_;
    }

    inline std::shared_ptr<AppExecFwk::EventRunner>& GetMainRunner()
    {
        return runner_;
    }

    MOCK_METHOD0(GetCurrentAccountData, sptr<AccessibilityAccountData>());
    MOCK_METHOD0(GetBundleMgrProxy, sptr<AppExecFwk::IBundleMgr>());
    MOCK_METHOD1(RemovedUser, void(int32_t accountId));
    MOCK_METHOD0(PresentUser, void());
    MOCK_METHOD1(PackageChanged, void(std::string& bundleName));
    MOCK_METHOD1(PackageRemoved, void(std::string& bundleName));
    MOCK_METHOD1(PackageAdd, void(std::string& bundleName));
    MOCK_METHOD1(PackageUpdateFinished, void(std::string& bundleName));
    MOCK_METHOD0(UpdateAccessibilityManagerService, void());
    MOCK_METHOD0(UpdateAbilities, void());
    MOCK_METHOD0(GetEnabledState, bool());
    MOCK_METHOD0(GetCaptionState, bool());
    MOCK_METHOD0(GetTouchGuideState, bool());
    MOCK_METHOD0(GetGestureState, bool());
    MOCK_METHOD0(GetKeyEventObserverState, bool());
    MOCK_METHOD1(SetTouchGuideState, bool(const bool state));
    MOCK_METHOD1(SetGestureState, bool(const bool state));
    MOCK_METHOD1(SetKeyEventObserverState, bool(const bool state));
    MOCK_METHOD1(SetEnabledObj, bool(std::map<std::string, AppExecFwk::ElementName> it));
    MOCK_METHOD0(GetEnabledAbilities, std::vector<std::string>());
    MOCK_METHOD0(GetInstalledAbilities, std::vector<AccessibilityAbilityInfo>());
    MOCK_METHOD1(RegisterUITestAbilityConnectionClient, bool(const sptr<IRemoteObject>& obj));
    MOCK_METHOD0(DeregisterUITestAbilityConnectionClient, bool());
    MOCK_METHOD0(GetActiveWindow, int32_t());
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_ACCESSIBLE_ABILITY_MANAGER_SERVICE_H