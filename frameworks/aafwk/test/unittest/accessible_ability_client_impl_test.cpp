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
#include "accessible_ability_client_impl.h"
#include "accessible_ability_manager_service.h"
#include "mock_accessible_ability_channel_proxy.h"
#include "mock_accessible_ability_channel_stub.h"
#include "mock_accessible_ability_listener.h"

using namespace testing;
using namespace testing::ext;

const static int32_t CHANNEL_ID = 1;
const static int32_t SEQUENCE = 1;
const static int32_t DISPLAY_ID = 1;
const static float SCALE = 1.0;
const static float CENTER_X = 1.0;
const static float CENTER_Y = 1.0;
const static int32_t FOCUS_TYPE = 1;
const static int32_t INDEX = 1;
const static std::string TEST = "test";

namespace OHOS {
namespace Accessibility {
class AccessibleAbilityClientImplTest : public ::testing::Test {
public:
    AccessibleAbilityClientImplTest()
    {}
    ~AccessibleAbilityClientImplTest()
    {}

    std::shared_ptr<AccessibleAbilityClientImpl> instance_ = nullptr;

    static void SetUpTestCase()
    {
        DelayedSingleton<AccessibleAbilityManagerService>::GetInstance()->OnStart();
        GTEST_LOG_(INFO) << "AccessibleAbilityClientImplTest Start";
    }
    static void TearDownTestCase()
    {
        DelayedSingleton<AccessibleAbilityManagerService>::GetInstance()->OnStop();
        GTEST_LOG_(INFO) << "AccessibleAbilityClientImplTest End";
    }
    void SetUp()
    {
        GTEST_LOG_(INFO) << "AccessibleAbilityClientImplTest SetUp()";
        instance_ = std::make_shared<AccessibleAbilityClientImpl>();
    };
    void TearDown()
    {
        GTEST_LOG_(INFO) << "AccessibleAbilityClientImplTest TearDown()";
        instance_ = nullptr;
    }
};

/**
 * @tc.number: Disconnect_001
 * @tc.name: Disconnect
 * @tc.desc: Test function Disconnect
 */
HWTEST_F(AccessibleAbilityClientImplTest, Disconnect_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Disconnect_001 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }

    instance_->Disconnect(CHANNEL_ID);

    GTEST_LOG_(INFO) << "Disconnect_001 end";
}

/**
 * @tc.number: OnAccessibilityEvent_001
 * @tc.name: OnAccessibilityEvent
 * @tc.desc: Test function OnAccessibilityEvent
 */
HWTEST_F(AccessibleAbilityClientImplTest, OnAccessibilityEvent_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "OnAccessibilityEvent_001 start";
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    AccessibilityEventInfo eventInfo {};
    instance_->OnAccessibilityEvent(eventInfo);
    GTEST_LOG_(INFO) << "OnAccessibilityEvent_001 end";
}

/**
 * @tc.number: OnDisplayResized_001
 * @tc.name: OnDisplayResized
 * @tc.desc: Test function OnDisplayResized
 */
HWTEST_F(AccessibleAbilityClientImplTest, OnDisplayResized_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "OnDisplayResized_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    Rect rect {};
    instance_->OnDisplayResized(DISPLAY_ID, rect, SCALE, CENTER_X, CENTER_Y);
    GTEST_LOG_(INFO) << "OnDisplayResized_001 end";
}

/**
 * @tc.number: OnGestureInjectResult_001
 * @tc.name: OnGestureInjectResult
 * @tc.desc: Test function OnGestureInjectResult
 */
HWTEST_F(AccessibleAbilityClientImplTest, OnGestureInjectResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "OnGestureInjectResult_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    instance_->OnGestureInjectResult(SEQUENCE, true);
    GTEST_LOG_(INFO) << "OnGestureInjectResult_001 end";
}

/**
 * @tc.number: GetFocus_001
 * @tc.name: GetFocus
 * @tc.desc: Test function GetFocus
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetFocus_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetFocus_001 start";
        std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get GetFocus instance_";
        return;
    }
    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info {};
    EXPECT_FALSE(instance_->GetFocus(FOCUS_TYPE, info));
    GTEST_LOG_(INFO) << "GetFocus_001 end";
}

/**
 * @tc.number: GetRemoteObject_001
 * @tc.name: GetRemoteObject
 * @tc.desc: Test function GetRemoteObject
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetRemoteObject_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetRemoteObject_001 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    sptr<IRemoteObject> res = nullptr;
    res = AccessibleAbilityClient::GetInstance()->GetRemoteObject();
    EXPECT_NE(res, nullptr);
    GTEST_LOG_(INFO) << "GetRemoteObject_001 end";
}

/**
 * @tc.number: RegisterAbilityListener_001
 * @tc.name: RegisterAbilityListener
 * @tc.desc: Test function RegisterAbilityListener
 */
HWTEST_F(AccessibleAbilityClientImplTest, RegisterAbilityListener_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "RegisterAbilityListener_001 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    std::shared_ptr<AccessibleAbilityListener> listener = nullptr;
    EXPECT_TRUE(instance_->RegisterAbilityListener(listener));

    GTEST_LOG_(INFO) << "RegisterAbilityListener_001 end";
}

/**
 * @tc.number: Init_001
 * @tc.name: Init
 * @tc.desc: Test function Init
 */
HWTEST_F(AccessibleAbilityClientImplTest, Init_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "Init_001 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    sptr<IAccessibleAbilityChannel> channel = nullptr;
    instance_->Init(channel, CHANNEL_ID);

    GTEST_LOG_(INFO) << "Init_001 end";
}

/**
 * @tc.number: GetFocusByElementInfo_001
 * @tc.name: GetFocusByElementInfo
 * @tc.desc: Test function GetFocusByElementInfo
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetFocusByElementInfo_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetFocusByElementInfo_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo sourceInfo {};
    AccessibilityElementInfo elementInfo {};
    EXPECT_FALSE(instance_->GetFocusByElementInfo(sourceInfo, FOCUS_TYPE, elementInfo));

    GTEST_LOG_(INFO) << "GetFocusByElementInfo_001 end";
}

/**
 * @tc.number: GestureInject_001
 * @tc.name: GestureInject
 * @tc.desc: Test function GestureInject
 */
HWTEST_F(AccessibleAbilityClientImplTest, GestureInject_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GestureInject_001 start";
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    std::vector<AccessibilityGesturePath> gesturePathList {};
    std::shared_ptr<AccessibilityGestureResultListener> listener = nullptr;
    EXPECT_FALSE(instance_->GestureInject(SEQUENCE, gesturePathList, listener));

    GTEST_LOG_(INFO) << "GestureInject_001 end";
}

/**
 * @tc.number: GetDisplayResizeController_001
 * @tc.name: GetDisplayResizeController
 * @tc.desc: Test function GetDisplayResizeController
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetDisplayResizeController_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetDisplayResizeController_001 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }

    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());
    int32_t channelId = 1;
    instance_->Init(channel, channelId);

    std::shared_ptr<DisplayResizeController> res = nullptr;
    res = instance_->GetDisplayResizeController();
    EXPECT_NE(res, nullptr);
    GTEST_LOG_(INFO) << "GetDisplayResizeController_001 end";
}

/**
 * @tc.number: GetDisplayResizeController_002
 * @tc.name: GetDisplayResizeController
 * @tc.desc: Test function GetDisplayResizeController
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetDisplayResizeController_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetDisplayResizeController_002 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    std::shared_ptr<DisplayResizeController> res = nullptr;
    res = instance_->GetDisplayResizeController(DISPLAY_ID);
    EXPECT_NE(res, nullptr);
    GTEST_LOG_(INFO) << "GetDisplayResizeController_002 end";
}

/**
 * @tc.number: GetRoot_001
 * @tc.name: GetRoot
 * @tc.desc: Test function GetRoot
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetRoot_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetRoot_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo info {};
    EXPECT_FALSE(instance_->GetRoot(info));

    GTEST_LOG_(INFO) << "GetRoot_001 end";
}

/**
 * @tc.number: GetRootByWindow_001
 * @tc.name: GetRootByWindow
 * @tc.desc: Test function GetRootByWindow
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetRootByWindow_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetRootByWindow_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo info {};
    AccessibilityWindowInfo windowInfo {};
    EXPECT_FALSE(instance_->GetRootByWindow(windowInfo, info));

    GTEST_LOG_(INFO) << "GetRootByWindow_001 end";
}

/**
 * @tc.number: ExecuteCommonAction_001
 * @tc.name: ExecuteCommonAction
 * @tc.desc: Test function ExecuteCommonAction
 */
HWTEST_F(AccessibleAbilityClientImplTest, ExecuteCommonAction_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "ExecuteCommonAction_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    GlobalAction action = GLOBAL_ACTION_INVALID;
    EXPECT_FALSE(instance_->ExecuteCommonAction(action));

    GTEST_LOG_(INFO) << "ExecuteCommonAction_001 end";
}

/**
 * @tc.number: GetNext_001
 * @tc.name: GetNext
 * @tc.desc: Test function GetNext
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetNext_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetNext_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo info {};
    AccessibilityElementInfo nextElementInfo {};
    FocusMoveDirection direction = DIRECTION_INVALID;
    EXPECT_FALSE(instance_->GetNext(info, direction, nextElementInfo));

    GTEST_LOG_(INFO) << "GetNext_001 end";
}

/**
 * @tc.number: GetChildWindowInfo_001
 * @tc.name: GetChildWindowInfo
 * @tc.desc: Test function GetChildWindowInfo
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetChildWindowInfo_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetChildWindowInfo_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityWindowInfo parent {};
    AccessibilityWindowInfo child {};
    EXPECT_FALSE(instance_->GetChildWindowInfo(INDEX, parent, child));

    GTEST_LOG_(INFO) << "GetChildWindowInfo_001 end";
}

/**
 * @tc.number: GetChildElementInfo_001
 * @tc.name: GetChildElementInfo
 * @tc.desc: Test function GetChildElementInfo
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetChildElementInfo_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetChildElementInfo_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo parent {};
    AccessibilityElementInfo child {};
    EXPECT_FALSE(instance_->GetChildElementInfo(INDEX, parent, child));

    GTEST_LOG_(INFO) << "GetChildElementInfo_001 end";
}

/**
 * @tc.number: GetByContent_001
 * @tc.name: GetByContent
 * @tc.desc: Test function GetByContent
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetByContent_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetByContent_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo elementInfo {};
    std::vector<AccessibilityElementInfo> inelementInfosfos;
    EXPECT_FALSE(instance_->GetByContent(elementInfo, TEST, inelementInfosfos));

    GTEST_LOG_(INFO) << "GetByContent_001 end";
}

/**
 * @tc.number: GetAnchor_001
 * @tc.name: GetAnchor
 * @tc.desc: Test function GetAnchor
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetAnchor_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetAnchor_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityWindowInfo windowInfo {};
    AccessibilityElementInfo elementInfo {};
    EXPECT_FALSE(instance_->GetAnchor(windowInfo, elementInfo));

    GTEST_LOG_(INFO) << "GetAnchor_001 end";
}

/**
 * @tc.number: GetSource_001
 * @tc.name: GetSource
 * @tc.desc: Test function GetSource
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetSource_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetSource_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityEventInfo eventInfo {};
    AccessibilityElementInfo elementInfo {};
    EXPECT_FALSE(instance_->GetSource(eventInfo, elementInfo));

    GTEST_LOG_(INFO) << "GetSource_001 end";
}

/**
 * @tc.number: GetParentWindowInfo_001
 * @tc.name: GetParentWindowInfo
 * @tc.desc: Test function GetParentWindowInfo
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetParentWindowInfo_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetParentWindowInfo_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityWindowInfo parent {};
    AccessibilityWindowInfo child {};
    EXPECT_TRUE(instance_->GetParentWindowInfo(child, parent));

    GTEST_LOG_(INFO) << "GetParentWindowInfo_001 end";
}

/**
 * @tc.number: GetParentElementInfo_001
 * @tc.name: GetParentElementInfo
 * @tc.desc: Test function GetParentElementInfo
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetParentElementInfo_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetParentElementInfo_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo parent {};
    AccessibilityElementInfo child {};
    EXPECT_FALSE(instance_->GetParentElementInfo(child, parent));

    GTEST_LOG_(INFO) << "GetParentElementInfo_001 end";
}

/**
 * @tc.number: GetLabeled_001
 * @tc.name: GetLabeled
 * @tc.desc: Test function GetLabeled
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetLabeled_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetLabeled_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo elementInfo {};
    AccessibilityElementInfo labeledElementInfo {};
    EXPECT_FALSE(instance_->GetLabeled(elementInfo, labeledElementInfo));

    GTEST_LOG_(INFO) << "GetLabeled_001 end";
}

/**
 * @tc.number: ExecuteAction_001
 * @tc.name: ExecuteAction
 * @tc.desc: Test function ExecuteAction
 */
HWTEST_F(AccessibleAbilityClientImplTest, ExecuteAction_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "ExecuteAction_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    AccessibilityElementInfo elementInfo {};
    ActionType action = ACCESSIBILITY_ACTION_INVALID;
    std::map<std::string, std::string> actionArguments {};
    EXPECT_FALSE(instance_->ExecuteAction(elementInfo, action, actionArguments));

    GTEST_LOG_(INFO) << "ExecuteAction_001 end";
}

/**
 * @tc.number: ResetAAClient_001
 * @tc.name: ResetAAClient
 * @tc.desc: Test function ResetAAClient
 */
HWTEST_F(AccessibleAbilityClientImplTest, ResetAAClient_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "ResetAAClient_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    wptr<IRemoteObject> remote = nullptr;
    instance_->ResetAAClient(remote);

    GTEST_LOG_(INFO) << "ResetAAClient_001 end";
}

/**
 * @tc.number: OnKeyPressEvent_001
 * @tc.name: OnKeyPressEvent
 * @tc.desc: Test function OnKeyPressEvent
 */
HWTEST_F(AccessibleAbilityClientImplTest, OnKeyPressEvent_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "OnKeyPressEvent_001 start";
    std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibleAbilityClientImpl instance_";
        return;
    }
    std::shared_ptr<MMI::KeyEvent> event = MMI::KeyEvent::Create();
    instance_->OnKeyPressEvent(*event, SEQUENCE);

    GTEST_LOG_(INFO) << "OnKeyPressEvent_001 end";
}

/**
 * @tc.number: GetWindows_001
 * @tc.name: GetWindows
 * @tc.desc: Test function GetWindows
 */
HWTEST_F(AccessibleAbilityClientImplTest, GetWindows_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "GetWindows_001 start";
        std::shared_ptr<AccessibleAbilityListener> listener = std::make_shared<MockAccessibleAbilityListener>();
    instance_->RegisterAbilityListener(listener);

    sptr<MockAccessibleAbilityChannelStub> stub = new MockAccessibleAbilityChannelStub();
    sptr<IAccessibleAbilityChannel> channel = new MockAccessibleAbilityChannelProxy(stub->AsObject());

    int32_t channelId = 1;
    instance_->Init(channel, channelId);
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get GetFocus instance_";
        return;
    }
    std::vector<AccessibilityWindowInfo> infos;
    infos = instance_->GetWindows();
    EXPECT_EQ(0, infos.size());
    GTEST_LOG_(INFO) << "GetWindows_001 end";
}
} // namespace Accessibility
} // namespace OHOS