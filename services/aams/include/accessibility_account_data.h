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
#ifndef ACCESSIBILITY_ACCOUNT_DATA_H
#define ACCESSIBILITY_ACCOUNT_DATA_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "accessibility_caption.h"
#include "accessibility_window_connection.h"
#include "accessible_ability_connection.h"
#include "i_accessible_ability_manager_caption_observer.h"
#include "i_accessible_ability_manager_state_observer.h"
#include "element_name.h"
#include "preferences_helper.h"

namespace OHOS {
namespace Accessibility {
using CaptionPropertyCallbacks = std::vector<sptr<IAccessibleAbilityManagerCaptionObserver>>;
class AccessibleAbilityConnection;
class AccessibilityWindowConnection;

enum STATE : int32_t {
    ACCESSIBILITY,
    TOUCHGUIDE,
    GESTURE,
    KEYEVENT,
    CAPTION
};

class AccessibilityAccountData final : public RefBase {
public:
    AccessibilityAccountData(int32_t accountId);
    ~AccessibilityAccountData();

    /**
     * @brief Get the ID of the account.
     * @param
     * @return Returns accountId.
     */
    int32_t GetAccountId();

    /**
     * @brief Get Accessibility state.
     * @param
     * @return returns whether accessibility is connected and whether can touch browse.
     */
    uint32_t GetAccessibilityState();

    /**
     * @brief Switch users and clear old user data.
     * @param
     * @return
     */
    void OnAccountSwitched();

    /**
     * @brief Add connected accessibility services.
     * @param connection Accessible ability connection.
     * @return
     */
    void AddConnectedAbility(sptr<AccessibleAbilityConnection>& connection);

    /**
     * @brief Remove connected accessibility services.
     * @param connection Accessible ability connection.
     * @return
     */
    void RemoveConnectedAbility(sptr<AccessibleAbilityConnection>& connection);

    /**
     * @brief Remove connecting accessibility services.
     * @param elementName Accessibility corresponding elementName.
     * @return
     */
    void RemoveConnectingA11yAbility(const std::string &bundleName);

    /**
     * @brief Add accessibility monitoring connection.
     * @param callback Accessibility monitoring connection.
     * @return
     */
    void AddStateCallback(const sptr<IAccessibleAbilityManagerStateObserver>& callback);

    /**
     * @brief Remove accessibility monitoring connection.
     * @param callback Accessibility monitoring connection.
     * @return
     */
    void RemoveStateCallback(const wptr<IRemoteObject>& callback);

    void AddCaptionPropertyCallback(const sptr<IAccessibleAbilityManagerCaptionObserver>& callback);
    void RemoveCaptionPropertyCallback(const wptr<IRemoteObject>& callback);
    /**
     * @brief Add interface operation interactive connection.
     * @param windowId Interface operation interactive connection the
     * corresponding window id.
     * @param interactionConnection Interface interface operation
     * interactive connection.
     * @return
     */
    void AddAccessibilityWindowConnection(
        const int32_t windowId, const sptr<AccessibilityWindowConnection>& interactionConnection);

    /**
     * @brief Remove interface operation interactive connection.
     * @param windowId Interface operation interactive connection the corresponding window id.
     * @return
     */
    void RemoveAccessibilityWindowConnection(const int32_t windowId);

    /**
     * @brief Add connecting accessibility.
     * @param elementName Accessibility corresponding elementName.
     * @return
     */
    void AddConnectingA11yAbility(const std::string &bundleName);

    void AddEnabledAbility(const std::string &bundleName); // For UT

    /**
     * @brief Remove accessibility that have been opened.
     * @param elementName Accessibility corresponding elementName.
     * @return
     */
    void RemoveEnabledAbility(const std::string &bundleName);

    void AddInstalledAbility(AccessibilityAbilityInfo& abilityInfo); // For UT

    /**
     * @brief Empty installed accessibility list.
     * @param abilityInfo Accessibility ability info.
     * @return
     */
    void RemoveInstalledAbility(std::string bundleName);

    /**
     * @brief The real procedure for add connecting ability.
     * @param
     * @return
     */
    void ClearInstalledAbility();

    /**
     * @brief Get connected accessibility list.
     * @param
     * @return Store map of connected accessibility abilities.
     */
    const std::map<std::string, sptr<AccessibleAbilityConnection>> GetConnectedA11yAbilities();

    /**
     * @brief Get accessibility listening connection list.
     * @param
     * @return Store vector of accessibility listening connection.
     */
    const std::vector<sptr<IAccessibleAbilityManagerStateObserver>> GetStateCallbacks();

    const CaptionPropertyCallbacks GetCaptionPropertyCallbacks();

    /**
     * @brief Get interface operation interactive connection list.
     * @param
     * @return Store map of interface operation interactive connection.
     */
    const std::map<int32_t, sptr<AccessibilityWindowConnection>> GetAsacConnections();

    /**
     * @brief Query accessible ability connection through elementName URI.
     * @param elementName Accessibility elementName URI
     * @return Accessible ability connection corresponding to elementName URI.
     */
    const sptr<AccessibleAbilityConnection> GetAccessibleAbilityConnection(const std::string elementName);

    /**
     * @brief Query interface operation interactive connection through window id.
     * @param windowId Interface operation interactive connection the corresponding window id.
     * @return Interface operation interactive connection corresponding to window id.
     */
    const sptr<AccessibilityWindowConnection> GetAccessibilityWindowConnection(const int32_t windowId);

    /**
     * @brief Get connecting abilities list.
     * @param
     * @return Store map of connecting abilities.
     */
    const std::vector<std::string> &GetConnectingA11yAbilities();

    /**
     * @brief Get the accessibility ability info of the corresponding state according to the
     *        ability state type.
     * @param state Ability state type.
     * @return
     */
    const std::vector<AccessibilityAbilityInfo> GetAbilitiesByState(AbilityStateType state);

    /**
     * @brief Get the accessibility ability info of the disabled ability.
     * @return
     */
    const std::vector<AccessibilityAbilityInfo> GetDisableAbilities();

    /**
     * @brief Get enable accessibility list.
     * @param
     * @return Store map of enable accessibility abilities.
     */
    const std::vector<std::string> &GetEnabledAbilities();

    /**
     * @brief Get install accessibility list.
     * @param
     * @return Store vector of install accessibility abilities.
     */
    const std::vector<AccessibilityAbilityInfo> GetInstalledAbilities();

    /**
     * @brief Update user enabled accessibility capabilities.
     * @param
     * @return
     */
    void UpdateAccountCapabilities();

    /**
     * @brief Read accessibility information in settings.
     * @param
     * @return Success return true, failed return false.
     */
    bool ReadConfigurationForAccountData();

    /**
     * @brief Get flag with whether have zoom capability.
     * @param
     * @return isScreenMagnification_.
     */
    bool GetScreenMagnificationFlag()
    {
        return isScreenMagnification_;
    }

    /**
     * @brief Get flag with whether have touch guide capability.
     * @param
     * @return isEventTouchGuideState_.
     */
    bool GetEventTouchGuideStateFlag()
    {
        return isEventTouchGuideState_;
    }

    /**
     * @brief Get flag with whether have key event observer capability.
     * @param
     * @return isFilteringKeyEvents_.
     */
    bool GetFilteringKeyEventsFlag()
    {
        return isFilteringKeyEvents_;
    }

    /**
     * @brief Get flag with whether have gesture capability.
     * @param
     * @return isGesturesSimulation_.
     */
    bool GetGesturesSimulationFlag()
    {
        return isGesturesSimulation_;
    }
    bool GetInstalledAbilitiesFromBMS();

    /**
     * @brief Add enabled event which is needed by accessible ability.
     * @param type EventType
     * @return null.
     */
    void AddEventEnabled(EventType type);

    /**
     * @brief Delet enabled event which is needed by accessible ability.
     * @param type EventType
     * @return null.
     */
    void DeleteEventEnabled(EventType type);

    CaptionProperty GetCaptionProperty()
    {
        return captionProperty_;
    };

    bool SetCaptionProperty(const CaptionProperty& caption);

    bool SetCaptionState(const bool state);

    bool SetEnabled(const bool state);

    bool SetTouchGuideState(const bool state);

    bool SetGestureState(const bool state);

    bool SetKeyEventObserverState(const bool state);

    bool GetEnabledState();

    bool GetTouchGuideState();

    bool GetGestureState();

    bool GetKeyEventObserverState();

    bool GetCaptionState();

    bool EnableAbilities(std::vector<std::string> &abilities);

    void init();

    bool DisableAbilities(const std::vector<std::string> &abilities);
private:
    /**
     * @brief Update connected accessibility whether have touch guide
     * capability. Have touch guide capability isEventTouchGuideState_ is
     * true, otherwise isEventTouchGuideState_ is false.
     * @param
     * @return
     */
    void UpdateEventTouchGuideCapability();

    /**
     * @brief Update connected accessibility whether have gesture capability.
     *        Have gesture capability isGesturesSimulation_ is true,
     *        otherwise isGesturesSimulation_ is false.
     * @param
     * @return
     */
    void UpdateGesturesSimulationCapability();

    /**
     * @brief Update connected accessibility whether have key event observer capability.
     *        Have key event observer capability isFilteringKeyEvents_ is true,
     *        otherwise isFilteringKeyEvents_ is false.
     * @param
     * @return
     */
    void UpdateFilteringKeyEventsCapability();

    /**
     * @brief Update connected accessibility whether have zoom capability.
     *        Have zoom capability isScreenMagnification_ is true,
     *        otherwise isScreenMagnification_ is false.
     * @param
     * @return
     */
    void UpdateMagnificationCapability();

    void CaptionInit(const std::shared_ptr<NativePreferences::Preferences> &pref);
    void CapabilityInit(const std::shared_ptr<NativePreferences::Preferences> &pref);
    void EnabledListInit(const std::shared_ptr<NativePreferences::Preferences> &pref);
    void StringToVector(std::string &stringIn, std::vector<std::string> &vectorResult);
    void VectorToString(std::vector<std::string> &vectorVal, std::string &stringOut);
    void RemoveEnabledFromPref(const std::string bundleName);
    void UpdateEnabledFromPref();
    bool SetStatePref(int32_t type);
    bool SetCaptionPropertyPref();
    std::string StateChange(bool state);

    int32_t id_;
    bool isEnabled_ = false;
    bool isEventTouchGuideState_ = false;
    bool isScreenMagnification_ = false;
    bool isFilteringKeyEvents_ = false;
    bool isGesturesSimulation_ = false;
    bool isCaptionState_ = false;
    CaptionProperty captionProperty_;
    std::map<std::string, sptr<AccessibleAbilityConnection>> connectedA11yAbilities_; // key: The URI of ElementName.
    std::vector<sptr<IAccessibleAbilityManagerStateObserver>> stateCallbacks_;
    std::map<int32_t, sptr<AccessibilityWindowConnection>> asacConnections_; // key: windowId
    CaptionPropertyCallbacks captionPropertyCallbacks_;
    std::vector<AccessibilityAbilityInfo> installedAbilities_;
    std::vector<std::string> enabledAbilities_; // The bundle name of enabled ability
    std::vector<std::string> connectingA11yAbilities_; // The bundle name of enabled ability
    std::shared_ptr<NativePreferences::Preferences> pref_ = nullptr;
};
} // namespace Accessibility
} // namespace OHOS

#endif  // ACCESSIBILITY_ACCOUNT_DATA_H