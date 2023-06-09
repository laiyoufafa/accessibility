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

#include <gtest/gtest.h>
#include "accessibility_ability_helper.h"
#include "accessibility_account_data.h"
#include "accessibility_event_info.h"
#include "accessible_ability_channel_stub.h"
#include "accessible_ability_client_proxy.h"
#include "accessible_ability_connection.h"
#include "accessible_ability_manager_service.h"
#include "parcel.h"

namespace OHOS {
namespace Accessibility {
AccessibleAbilityClientProxy::AccessibleAbilityClientProxy(const sptr<IRemoteObject>& object)
    : IRemoteProxy<IAccessibleAbilityClient>(object)
{
    (void)object;
}

bool AccessibleAbilityClientProxy::WriteInterfaceToken(MessageParcel& data)
{
    (void)data;
    return true;
}

void AccessibleAbilityClientProxy::Init(const sptr<IAccessibleAbilityChannel>& channel, const int32_t channelId)
{
    (void)channel;
    AccessibilityAbilityHelper::GetInstance().SetTestChannelId(channelId);
}

void AccessibleAbilityClientProxy::Disconnect(const int32_t channelId)
{
    (void)channelId;
}

void AccessibleAbilityClientProxy::OnAccessibilityEvent(const AccessibilityEventInfo& eventInfo)
{
    GTEST_LOG_(INFO) << "TouchGuider_Unittest_OnTouchEvent OnAccessibilityEvent";
    AccessibilityAbilityHelper::GetInstance().SetTestEventType(int(eventInfo.GetEventType()));
    AccessibilityAbilityHelper::GetInstance().SetGestureId(int(eventInfo.GetGestureType()));

    AccessibilityAbilityHelper::GetInstance().SetEventTypeVector(eventInfo.GetEventType());
    GTEST_LOG_(INFO) << "AccessibleAbilityClientProxy::OnAccessibilityEvent eventType="
                     << (int32_t)eventInfo.GetEventType();
}

void AccessibleAbilityClientProxy::OnKeyPressEvent(const MMI::KeyEvent& keyEvent, const int32_t sequence)
{
    (void)keyEvent;
    MessageParcel data;
    MessageParcel reply;
    HILOG_DEBUG();
    HILOG_DEBUG("start.-----sequence--%{public}d ----------mock Proxy Start ", sequence);
    AccessibilityAbilityHelper::GetInstance().SetTestKeyPressEvent(sequence);
    HILOG_DEBUG("start.-----------------------------mock Proxy end ");
}

void AccessibleAbilityClientProxy::OnDisplayResized(
    const int32_t displayId, const Rect& rect, const float scale, const float centerX, const float centerY)
{
    (void)rect;
    (void)displayId;
    (void)scale;
    (void)centerX;
    (void)centerY;
    AccessibilityAbilityHelper::GetInstance().SetTestDisplayId(1);
}

void AccessibleAbilityClientProxy::OnGestureInjectResult(const int32_t sequence, const bool completedSuccessfully)
{
    (void)sequence;
    (void)completedSuccessfully;
    AccessibilityAbilityHelper::GetInstance().SetTestGestureSimulateResult(1);
}
} // namespace Accessibility
} // namespace OHOS