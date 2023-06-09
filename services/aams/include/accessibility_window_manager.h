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

#ifndef ACCESSIBILITY_WINDOW_MANGER_H
#define ACCESSIBILITY_WINDOW_MANGER_H

#include <map>
#include <memory>
#include "accessibility_window_info.h"
#include "event_handler.h"
#include "singleton.h"
#include "window_manager.h"

namespace OHOS {
namespace Accessibility {
class AccessibilityWindowManager {
public:
    ~AccessibilityWindowManager() = default;
    static AccessibilityWindowManager &GetInstance();
    static AccessibilityWindowInfo CreateAccessibilityWindowInfo(Rosen::AccessibilityWindowInfo &windowInfo);
    int32_t ConvertToRealWindowId(int32_t windowId, int32_t focusType);
    void RegisterWindowChangeListener();
    void DeregisterWindowChangeListener();
    void SetActiveWindow(int32_t windowId);
    void SetAccessibilityFocusedWindow(int32_t windowId);
    void SetInputFocusedWindow(int32_t windowId);
    std::vector<AccessibilityWindowInfo> GetAccessibilityWindows();
    bool GetAccessibilityWindow(int32_t windowId, AccessibilityWindowInfo &window);
    bool IsValidWindow(int32_t windowId);

    // test for ut to resize a window
    void SetWindowSize(int32_t windowId, Rect rect);
    void UpdateWindowLayer(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo);

    void OnWindowUpdate(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo, Rosen::WindowUpdateType type);

    std::map<int32_t, AccessibilityWindowInfo> a11yWindows_ {};
    int32_t activeWindowId_ = INVALID_WINDOW_ID;
    int32_t a11yFocusedWindowId_ = INVALID_WINDOW_ID;
    int32_t inputFocusedWindowId_ = 0;

    bool registerFlag_ = false;

private:
    class AccessibilityWindowListener : public Rosen::IWindowUpdateListener {
    public:
        explicit AccessibilityWindowListener(AccessibilityWindowManager &windInfoMgr)
            : windInfoMgr_(windInfoMgr) {}
        ~AccessibilityWindowListener() = default;

        virtual void OnWindowUpdate(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo,
            Rosen::WindowUpdateType type) override
        {
            windInfoMgr_.OnWindowUpdate(windowInfo, type);
        }

    private:
        AccessibilityWindowManager &windInfoMgr_;
    };

    AccessibilityWindowManager();
    void WindowUpdate(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo, Rosen::WindowUpdateType type);
    void WindowUpdateAdded(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo);
    void WindowUpdateRemoved(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo);

    sptr<AccessibilityWindowListener> windowListener_ = nullptr;
    std::shared_ptr<AppExecFwk::EventHandler> eventHandler_ = nullptr;
    DISALLOW_COPY_AND_MOVE(AccessibilityWindowManager);
};
} // namespace Accessibility
} // namespace OHOS
#endif  // ACCESSIBILITY_WINDOW_MANGER_H