/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed On an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "accessibility_window_manager.h"
#include "accessible_ability_manager_service.h"

namespace OHOS {
namespace Accessibility {

void AccessibilityWindowListener::OnWindowUpdate(const sptr<Rosen::WindowInfo>& windowInfo, Rosen::WindowUpdateType type)
{
    HILOG_DEBUG("%{public}s: windowId[%{public}d] type[%{public}d]", __func__, windowInfo->wid_, type);
    auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
    auto accountData = aams->GetCurrentAccountData();
    if (accountData->GetAccessibilityWindowConnection(windowInfo->wid_) == nullptr) {
        HILOG_ERROR("GetAccessibilityWindowConnection failed by window id(%{public}d)!!", windowInfo->wid_);
        return;
    }

    auto& winMgr = AccessibilityWindowInfoManager::GetInstance();
    switch (type) {
        case Rosen::WindowUpdateType::WINDOW_UPDATE_ADDED:
        {
            auto a11yWindowInfo = winMgr.CreateAccessibilityWindowInfo(*windowInfo);
            winMgr.a11yWindows_.emplace(windowInfo->wid_, a11yWindowInfo);
            AccessibilityEventInfo evtInf(windowInfo->wid_, WINDOW_UPDATE_ADDED);
            aams->SendEvent(evtInf, aams->GetCurrentAccountId());
            if (a11yWindowInfo.IsFocused()) {
                winMgr.SetActiveWindow(windowInfo->wid_);
            }
        }
        break;
        case Rosen::WindowUpdateType::WINDOW_UPDATE_REMOVED:
        {
            if (!winMgr.a11yWindows_.count(windowInfo->wid_)) {
                return;
            }
            if (windowInfo->wid_ == winMgr.activeWindowId_) {
                winMgr.SetActiveWindow(INVALID_WINDOW_ID);
            }
            if (windowInfo->wid_ == winMgr.a11yFocusedWindowId_) {
                winMgr.SetAccessibilityFocusedWindow(INVALID_WINDOW_ID);
            }
            if (windowInfo->wid_ == winMgr.inputFocusedWindowId_) {
                winMgr.SetInputFocusedWindow(INVALID_WINDOW_ID);
            }
            winMgr.a11yWindows_.erase(windowInfo->wid_);
            AccessibilityEventInfo evtInf(windowInfo->wid_, WINDOW_UPDATE_REMOVED);
            aams->SendEvent(evtInf, aams->GetCurrentAccountId());
        }
        break;
        case Rosen::WindowUpdateType::WINDOW_UPDATE_BOUNDS:
        {
            AccessibilityEventInfo evtInf(windowInfo->wid_, WINDOW_UPDATE_BOUNDS);
            aams->SendEvent(evtInf, aams->GetCurrentAccountId());
        }
        break;
        case Rosen::WindowUpdateType::WINDOW_UPDATE_ACTIVE:
            winMgr.SetActiveWindow(windowInfo->wid_);
        break;
        case Rosen::WindowUpdateType::WINDOW_UPDATE_FOCUSED:
        {
            winMgr.SetActiveWindow(windowInfo->wid_);
            AccessibilityEventInfo evtInf(windowInfo->wid_, WINDOW_UPDATE_FOCUSED);
            aams->SendEvent(evtInf, aams->GetCurrentAccountId());
        }
        break;
        default:
        break;
    }
    HILOG_DEBUG("%{public}s: winMgr.a11yWindows[%{public}d]", __func__, winMgr.a11yWindows_.size());
}

AccessibilityWindowInfoManager &AccessibilityWindowInfoManager::GetInstance()
{
    static AccessibilityWindowInfoManager windowManager;
    return windowManager;
}

int AccessibilityWindowInfoManager::ConvertToRealWindowId(int windowId, int focusType)
{
    HILOG_DEBUG("ConvertToRealWindowId called, windowId[%{public}d], focusType[%{public}d]", windowId, focusType);
    if (windowId == ACTIVE_WINDOW_ID) {
        HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", activeWindowId_);
        return activeWindowId_;
    }

    if (windowId == ANY_WINDOW_ID) {
        if (focusType == FOCUS_TYPE_ACCESSIBILITY) {
            HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", a11yFocusedWindowId_);
            return a11yFocusedWindowId_;
        } else if (focusType == FOCUS_TYPE_INPUT) {
            HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", inputFocusedWindowId_);
            return inputFocusedWindowId_;
        }
    }
    HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", windowId);
    return windowId;
}

WindowType ConvertWindowType(Rosen::WindowType type)
{
    WindowType winType = TYPE_WINDOW_INVALID;

    if (type < Rosen::WindowType::SYSTEM_WINDOW_BASE) {
        winType = TYPE_APPLICATION;
    } else if ((type >= Rosen::WindowType::SYSTEM_WINDOW_BASE) && (type <= Rosen::WindowType::SYSTEM_WINDOW_END)) {
        winType = TYPE_SYSTEM;
    }
    else{
        HILOG_ERROR("%{public}s: Unknown windowType[%{public}d]", __func__, type);
    }
    return winType;
}

AccessibilityWindowInfo AccessibilityWindowInfoManager::CreateAccessibilityWindowInfo(Rosen::WindowInfo &windowInfo)
{
    AccessibilityWindowInfo info;
    info.SetWindowId(windowInfo.wid_);
    info.SetWindowType(ConvertWindowType(windowInfo.type_));
    info.SetFocused(windowInfo.focused_);
    Rect bound;
    bound.SetLeftTopScreenPostion(windowInfo.windowRect_.posX_, windowInfo.windowRect_.posY_);
    bound.SetRightBottomScreenPostion(windowInfo.windowRect_.posX_ + windowInfo.windowRect_.width_,
                                      windowInfo.windowRect_.posY_ + windowInfo.windowRect_.height_);
    info.SetRectInScreen(bound);
    return info;
}

void AccessibilityWindowInfoManager::RegisterWindowChangeListener()
{
    if (!registerFlag_) {
        OHOS::Rosen::WindowManager::GetInstance().RegisterWindowUpdateListener(windowListener_);
        registerFlag_ = true;
    }
    return;
}

void AccessibilityWindowInfoManager::DeregisterWindowChangeListener()
{
    if (registerFlag_) {
        OHOS::Rosen::WindowManager::GetInstance().UnregisterWindowUpdateListener(windowListener_);
        registerFlag_ = false;
    }
    return;
}

void AccessibilityWindowInfoManager::SetActiveWindow(int windowId)
{
    HILOG_DEBUG("%{public}s start windowId(%{public}d)", __func__, windowId);
    if (windowId == INVALID_WINDOW_ID) {
        if (a11yWindows_.count(activeWindowId_)) {
            a11yWindows_[activeWindowId_].SetActive(false);
        }
        activeWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_WARN("Window id is not found");
        return;
    }

    if (activeWindowId_ != windowId) {
        if (a11yWindows_.count(activeWindowId_)) {
            a11yWindows_[activeWindowId_].SetActive(false);
        }
        activeWindowId_ = windowId;
        a11yWindows_[activeWindowId_].SetActive(true);
        auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
        AccessibilityEventInfo evtInf(activeWindowId_, WINDOW_UPDATE_ACTIVE);
        aams->SendEvent(evtInf, aams->GetCurrentAccountId());
    }
    HILOG_DEBUG("%{public}s %{public}d", __func__, activeWindowId_);
}

void AccessibilityWindowInfoManager::SetAccessibilityFocusedWindow(int windowId)
{
    HILOG_DEBUG("%{public}s start", __func__);
    if (windowId == INVALID_WINDOW_ID) {
        if (a11yWindows_.count(a11yFocusedWindowId_)) {
            a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(false);
        }
        a11yFocusedWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_WARN("Window id is not found");
        return;
    }

    if (a11yFocusedWindowId_ != windowId) {
        if (a11yWindows_.count(a11yFocusedWindowId_)) {
            a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(false);
        }
        a11yFocusedWindowId_ = windowId;
        a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(true);
    }
    HILOG_DEBUG("%{public}s %{public}d", __func__, a11yFocusedWindowId_);
}

void AccessibilityWindowInfoManager::SetInputFocusedWindow(int windowId)
{
    if (windowId == INVALID_WINDOW_ID) {
        if (a11yWindows_.count(inputFocusedWindowId_)) {
            a11yWindows_[inputFocusedWindowId_].SetFocused(false);
        }
        inputFocusedWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_WARN("Window id is not found");
        return;
    }

    if (inputFocusedWindowId_ != windowId) {
        if (a11yWindows_.count(inputFocusedWindowId_)) {
            a11yWindows_[inputFocusedWindowId_].SetFocused(false);
        }
        inputFocusedWindowId_ = windowId;
        a11yWindows_[inputFocusedWindowId_].SetFocused(true);
    }
}

std::vector<AccessibilityWindowInfo> AccessibilityWindowInfoManager::GetAccessibilityWindows()
{
    HILOG_INFO("%{public}s", __func__);
    HILOG_DEBUG("a11yWindows_ size[%{public}d]", a11yWindows_.size());
    std::vector<AccessibilityWindowInfo> windows;
    for (auto window : a11yWindows_) {
        windows.push_back(window.second);
    }
    HILOG_DEBUG("window size[%{public}d]", windows.size());
    for (auto logWindow : windows) {
        HILOG_DEBUG("logWindow id[%{public}d]", logWindow.GetWindowId());
    }
    return windows;
}

bool AccessibilityWindowInfoManager::GetAccessibilityWindow(int windowId, AccessibilityWindowInfo &window)
{
    HILOG_INFO("%{public}s", __func__);
    if (a11yWindows_.count(windowId)) {
        window = a11yWindows_[windowId];
        return true;
    }
    return false;
}

bool AccessibilityWindowInfoManager::IsValidWindow(int windowId)
{
    HILOG_INFO("%{public}s", __func__);
    if (a11yWindows_.count(windowId)) {
        return true;
    }
    return false;
}

void AccessibilityWindowInfoManager::SetWindowSize(int windowId, Rect rect)
{
    HILOG_INFO("%{public}s", __func__);
    for (auto &window : a11yWindows_) {
        if (window.first == windowId) {
            window.second.SetRectInScreen(rect);
            return;
        }
    }
}
}  // namespace Accessibility
}  // namespace OHOS