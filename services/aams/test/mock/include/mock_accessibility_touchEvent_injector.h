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

#ifndef MOCK_ACCESSIBILITY_TOUCHEVENT_INJECTOR_H
#define MOCK_ACCESSIBILITY_TOUCHEVENT_INJECTOR_H

#include <gmock/gmock.h>
#include "accessibility_touchEvent_injector.h"

namespace OHOS {
namespace Accessibility {
class MockTouchInjectHandler : public TouchInjectHandler {
public:
    MockTouchInjectHandler(const std::shared_ptr<AppExecFwk::EventRunner>& runner, TouchEventInjector& server);
    virtual ~MockTouchInjectHandler() = default;

    MOCK_METHOD1(ProcessEvent, void(const AppExecFwk::InnerEvent::Pointer& event));
};

class MockTouchEventInjector : public TouchEventInjector {
public:
    MockTouchEventInjector();
    ~MockTouchEventInjector()
    {}

    MOCK_METHOD1(OnPointerEvent, void(MMI::PointerEvent& event));
    MOCK_METHOD1(ClearEvents, void(uint32_t inputSource));
    MOCK_METHOD0(DestroyEvents, void());
    MOCK_METHOD3(InjectEvents, void(const std::vector<GesturePathDefine>& gesturePath,
                                    const sptr<IAccessibleAbilityClient>& service, int32_t sequence));
    MOCK_METHOD1(SendPointerEvent, void(MMI::PointerEvent& event));
    MOCK_METHOD0(InjectEventsInner, void());
    MOCK_METHOD0(GetCurrentGestureService, sptr<IAccessibleAbilityClient>());
    MOCK_METHOD0(GetSequence, int32_t());
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_ACCESSIBILITY_TOUCHEVENT_INJECTOR_H