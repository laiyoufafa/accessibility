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

#include "accessible_ability_channel_proxy.h"
#include "accessibility_element_info_parcel.h"
#include "accessibility_gesture_path_parcel.h"
#include "accessibility_window_info_parcel.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
AccessibleAbilityChannelProxy::AccessibleAbilityChannelProxy(
    const sptr<IRemoteObject> &object): IRemoteProxy<IAccessibleAbilityChannel>(object)
{
}

bool AccessibleAbilityChannelProxy::WriteInterfaceToken(MessageParcel &data)
{
    HILOG_DEBUG("start.");

    if (!data.WriteInterfaceToken(AccessibleAbilityChannelProxy::GetDescriptor())) {
        HILOG_ERROR("write interface token failed");
        return false;
    }
    return true;
}

bool AccessibleAbilityChannelProxy::SendTransactCmd(IAccessibleAbilityChannel::Message code,
    MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOG_DEBUG("start.");

    sptr<IRemoteObject> remote = Remote();
    if (!remote) {
        HILOG_ERROR("fail to send transact cmd %{public}d due to remote object", code);
        return false;
    }
    int32_t result = remote->SendRequest(static_cast<uint32_t>(code), data, reply, option);
    if (result != NO_ERROR) {
        HILOG_ERROR("receive error transact code %{public}d in transact cmd %{public}d", result, code);
        return false;
    }
    return true;
}

bool AccessibleAbilityChannelProxy::SearchElementInfoByAccessibilityId(const int32_t accessibilityWindowId,
    const int32_t elementId, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback,
    const int32_t mode)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(accessibilityWindowId)) {
        HILOG_ERROR("accessibilityWindowId write error: %{public}d, ", accessibilityWindowId);
        return false;
    }
    if (!data.WriteInt32(elementId)) {
        HILOG_ERROR("elementId write error: %{public}d, ", elementId);
        return false;
    }
    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("requestId write error: %{public}d, ", requestId);
        return false;
    }
    if (!callback) {
        HILOG_ERROR("callback is nullptr.");
        return false;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        HILOG_ERROR("callback write error");
        return false;
    }
    if (!data.WriteInt32(mode)) {
        HILOG_ERROR("mode write error: %{public}d, ", mode);
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::SEARCH_ELEMENTINFO_BY_ACCESSIBILITY_ID,
        data, reply, option)) {
        HILOG_ERROR("fail to find elementInfo by elementId");
        return false;
    }
    return reply.ReadBool();
}

bool AccessibleAbilityChannelProxy::SearchElementInfosByText(const int32_t accessibilityWindowId,
    const int32_t elementId, const std::string &text, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(accessibilityWindowId)) {
        HILOG_ERROR("accessibilityWindowId write error: %{public}d, ", accessibilityWindowId);
        return false;
    }
    if (!data.WriteInt32(elementId)) {
        HILOG_ERROR("elementId write error: %{public}d, ", elementId);
        return false;
    }
    if (!data.WriteString(text)) {
        HILOG_ERROR("text write error: %{public}s, ", text.c_str());
        return false;
    }
    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("requestId write error: %{public}d, ", requestId);
        return false;
    }
    if (!callback) {
        HILOG_ERROR("callback is nullptr.");
        return false;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        HILOG_ERROR("callback write error");
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::SEARCH_ELEMENTINFOS_BY_TEXT,
        data, reply, option)) {
        HILOG_ERROR("fail to find elementInfo by text");
        return false;
    }
    return reply.ReadBool();
}

bool AccessibleAbilityChannelProxy::FindFocusedElementInfo(const int32_t accessibilityWindowId,
    const int32_t elementId, const int32_t focusType, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(accessibilityWindowId)) {
        HILOG_ERROR("accessibilityWindowId write error: %{public}d, ", accessibilityWindowId);
        return false;
    }
    if (!data.WriteInt32(elementId)) {
        HILOG_ERROR("elementId write error: %{public}d, ", elementId);
        return false;
    }
    if (!data.WriteInt32(focusType)) {
        HILOG_ERROR("focusType write error: %{public}d, ", focusType);
        return false;
    }
    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("requestId write error: %{public}d, ", requestId);
        return false;
    }
    if (!callback) {
        HILOG_ERROR("callback is nullptr.");
        return false;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        HILOG_ERROR("callback write error");
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::FIND_FOCUSED_ELEMENTINFO, data, reply, option)) {
        HILOG_ERROR("fail to gain focus");
        return false;
    }
    return reply.ReadBool();
}

bool AccessibleAbilityChannelProxy::FocusMoveSearch(const int32_t accessibilityWindowId, const int32_t elementId,
    const int32_t direction, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(accessibilityWindowId)) {
        HILOG_ERROR("accessibilityWindowId write error: %{public}d, ", accessibilityWindowId);
        return false;
    }
    if (!data.WriteInt32(elementId)) {
        HILOG_ERROR("elementId write error: %{public}d, ", elementId);
        return false;
    }
    if (!data.WriteInt32(direction)) {
        HILOG_ERROR("direction write error: %{public}d, ", direction);
        return false;
    }
    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("requestId write error: %{public}d, ", requestId);
        return false;
    }
    if (!callback) {
        HILOG_ERROR("callback is nullptr.");
        return false;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        HILOG_ERROR("callback write error");
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::FOCUS_MOVE_SEARCH, data, reply, option)) {
        HILOG_ERROR("fail to search focus");
        return false;
    }
    return reply.ReadBool();
}

bool AccessibleAbilityChannelProxy::ExecuteAction(const int32_t accessibilityWindowId, const int32_t elementId,
    const int32_t action, std::map<std::string, std::string> &actionArguments, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(accessibilityWindowId)) {
        HILOG_ERROR("accessibilityWindowId write error: %{public}d, ", accessibilityWindowId);
        return false;
    }
    if (!data.WriteInt32(elementId)) {
        HILOG_ERROR("elementId write error: %{public}d, ", elementId);
        return false;
    }
    if (!data.WriteInt32(action)) {
        HILOG_ERROR("action write error: %{public}d, ", action);
        return false;
    }

    std::vector<std::string> actionArgumentsKey {};
    std::vector<std::string> actionArgumentsValue {};
    for (auto iter = actionArguments.begin(); iter != actionArguments.end(); iter++) {
        actionArgumentsKey.push_back(iter->first);
        actionArgumentsValue.push_back(iter->second);
    }
    if (!data.WriteStringVector(actionArgumentsKey)) {
        HILOG_ERROR("actionArgumentsKey write error");
        return false;
    }
    if (!data.WriteStringVector(actionArgumentsValue)) {
        HILOG_ERROR("actionArgumentsValue write error");
        return false;
    }

    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("requestId write error: %{public}d, ", requestId);
        return false;
    }
    if (!callback || !data.WriteRemoteObject(callback->AsObject())) {
        HILOG_ERROR("callback write error");
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::PERFORM_ACTION,
        data, reply, option)) {
        HILOG_ERROR("fail to perform accessibility action");
        return false;
    }
    return reply.ReadBool();
}

std::vector<AccessibilityWindowInfo> AccessibleAbilityChannelProxy::GetWindows()
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);
    std::vector<AccessibilityWindowInfo> windowsError;
    std::vector<AccessibilityWindowInfo> windows;

    if (!WriteInterfaceToken(data)) {
        return windowsError;
    }
    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::GET_WINDOWS, data, reply, option)) {
        HILOG_ERROR("fail to get windows");
        return windowsError;
    }

    int32_t windowsSize = reply.ReadInt32();
    for (int32_t i = 0; i < windowsSize; i++) {
        sptr<AccessibilityWindowInfoParcel> window = reply.ReadStrongParcelable<AccessibilityWindowInfoParcel>();
        if (!window) {
            HILOG_ERROR("ReadStrongParcelable<AccessibilityWindowInfoParcel> failed");
            return windowsError;
        }
        windows.emplace_back(*window);
    }

    return windows;
}

bool AccessibleAbilityChannelProxy::ExecuteCommonAction(const int32_t action)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(action)) {
        HILOG_ERROR("action write error: %{public}d, ", action);
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::EXECUTE_COMMON_ACTION, data, reply, option)) {
        HILOG_ERROR("fail to perform common action");
        return false;
    }
    return reply.ReadBool();
}

void AccessibleAbilityChannelProxy::SetOnKeyPressEventResult(const bool handled, const int32_t sequence)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return;
    }
    if (!data.WriteBool(handled)) {
        HILOG_ERROR("handled write error: %{public}d, ", handled);
        return;
    }
    if (!data.WriteInt32(sequence)) {
        HILOG_ERROR("sequence write error: %{public}d, ", sequence);
        return;
    }
    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::SET_ON_KEY_PRESS_EVENT_RESULT,
        data, reply, option)) {
        HILOG_ERROR("fail to set onKeyPressEvent result");
    }
}

float AccessibleAbilityChannelProxy::GetDisplayResizeScale(const int32_t displayId)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return 0;
    }
    if (!data.WriteInt32(displayId)) {
        HILOG_ERROR("displayId write error: %{public}d, ", displayId);
        return 0;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::GET_DISPALYRESIZE_SCALE, data, reply, option)) {
        HILOG_ERROR("fail to get displayResize scale");
        return 0;
    }
    return reply.ReadFloat();
}

float AccessibleAbilityChannelProxy::GetDisplayResizeCenterX(const int32_t displayId)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return 0;
    }
    if (!data.WriteInt32(displayId)) {
        HILOG_ERROR("displayId write error: %{public}d, ", displayId);
        return 0;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::GET_DISPALYRESIZE_CENTER_X, data, reply, option)) {
        HILOG_ERROR("fail to get displayResize centerX");
        return 0;
    }
    return reply.ReadFloat();
}

float AccessibleAbilityChannelProxy::GetDisplayResizeCenterY(const int32_t displayId)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return 0;
    }
    if (!data.WriteInt32(displayId)) {
        HILOG_ERROR("displayId write error: %{public}d, ", displayId);
        return 0;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::GET_DISPLAYRESIZE_CENTER_Y, data, reply, option)) {
        HILOG_ERROR("fail to get displayResize centerY");
        return 0;
    }
    return reply.ReadFloat();
}

Rect AccessibleAbilityChannelProxy::GetDisplayResizeRect(const int32_t displayId)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);
    Rect rect(0, 0, 0, 0);

    if (!WriteInterfaceToken(data)) {
        return rect;
    }
    if (!data.WriteInt32(displayId)) {
        HILOG_ERROR("displayId write error: %{public}d, ", displayId);
        return rect;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::GET_DISPLAYRESIZE_RECT, data, reply, option)) {
        HILOG_ERROR("fail to get displayResize rect");
        return rect;
    }

    sptr<RectParcel> result = reply.ReadStrongParcelable<RectParcel>();
    if (!result) {
        HILOG_ERROR("ReadStrongParcelable<Rect> failed");
        return rect;
    }
    return *result;
}

bool AccessibleAbilityChannelProxy::ResetDisplayResize(const int32_t displayId, const bool animate)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(displayId)) {
        HILOG_ERROR("displayId write error: %{public}d, ", displayId);
        return false;
    }
    if (!data.WriteBool(animate)) {
        HILOG_ERROR("animate write error: %{public}d, ", animate);
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::RESET_DISPALYRESIZE, data, reply, option)) {
        HILOG_ERROR("fail to reset displayResize");
        return false;
    }
    return reply.ReadBool();
}

bool AccessibleAbilityChannelProxy::SetDisplayResizeScaleAndCenter(const int32_t displayId, const float scale,
    const float centerX, const float centerY, const bool animate)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!WriteInterfaceToken(data)) {
        return false;
    }
    if (!data.WriteInt32(displayId)) {
        HILOG_ERROR("displayId write error: %{public}d, ", displayId);
        return false;
    }
    if (!data.WriteFloat(scale)) {
        HILOG_ERROR("scale write error: %{public}f, ", scale);
        return false;
    }
    if (!data.WriteFloat(centerX)) {
        HILOG_ERROR("centerX write error: %{public}f, ", centerX);
        return false;
    }
    if (!data.WriteFloat(centerY)) {
        HILOG_ERROR("centerY write error: %{public}f, ", centerY);
        return false;
    }
    if (!data.WriteBool(animate)) {
        HILOG_ERROR("animate write error: %{public}d, ", animate);
        return false;
    }

    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::SET_DISPLAYRESIZE_SCALE_AND_CENTER,
        data, reply, option)) {
        HILOG_ERROR("fail to set displayResize scale and center");
        return false;
    }
    return reply.ReadBool();
}

void AccessibleAbilityChannelProxy::SendSimulateGesture(const int32_t requestId,
    const std::vector<AccessibilityGesturePath> &gestureSteps)
{
    HILOG_DEBUG("start.");

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);

    if (!WriteInterfaceToken(data)) {
        return;
    }
    if (!data.WriteInt32(requestId)) {
        HILOG_ERROR("requestId write error: %{public}d, ", requestId);
        return;
    }
    if (!data.WriteInt32(gestureSteps.size())) {
        HILOG_ERROR("gestureSteps.size() write error: %{public}zu, ", gestureSteps.size());
        return;
    }
    for (auto &step : gestureSteps) {
        sptr<AccessibilityGesturePathParcel> path = new AccessibilityGesturePathParcel(step);
        if (!data.WriteStrongParcelable(path)) {
            HILOG_ERROR("WriteStrongParcelable<AccessibilityGesturePathParcel> failed");
            return;
        }
    }
    if (!SendTransactCmd(IAccessibleAbilityChannel::Message::SEND_SIMULATE_GESTURE, data, reply, option)) {
        HILOG_ERROR("fail to send simulation gesture");
    }
}
} // namespace Accessibility
} // namespace OHOS