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

#include "accessibility_input_interceptor.h"
#include "accessibility_keyevent_filter.h"
#include "accessibility_touch_guider.h"
#include "accessibility_touchEvent_injector.h"
#include "accessibility_zoom_handler.h"
#include "accessible_ability_manager_service.h"
#include "hilog_wrapper.h"
#include "key_event.h"
#include "input_event.h"

namespace OHOS {
namespace Accessibility {
sptr<AccessibilityInputInterceptor> AccessibilityInputInterceptor::instance_ = nullptr;
sptr<AccessibilityInputInterceptor> AccessibilityInputInterceptor::GetInstance()
{
    HILOG_DEBUG();

    if (!instance_) {
        instance_ = new(std::nothrow) AccessibilityInputInterceptor();
        if (!instance_) {
            HILOG_ERROR("instance_ is null");
            return nullptr;
        }
    }
    return instance_;
}

AccessibilityInputInterceptor::AccessibilityInputInterceptor()
{
    HILOG_DEBUG();

    aams_ = nullptr;
    pointerEventTransmitters_ = nullptr;
    keyEventTransmitters_ = nullptr;
    aams_ = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
    if (!aams_) {
        HILOG_ERROR("aams_ is null.");
        return;
    }
    inputManager_ = MMI::InputManager::GetInstance();
    eventHandler_ = std::make_shared<AppExecFwk::EventHandler>(aams_->GetMainRunner());
}

AccessibilityInputInterceptor::~AccessibilityInputInterceptor()
{
    HILOG_DEBUG();

    DestroyInterceptor();
    DestroyTransmitters();
    aams_ = nullptr;
    pointerEventTransmitters_ = nullptr;
    keyEventTransmitters_ = nullptr;
    interceptorId_ = -1;
    keyEventInterceptorId_ = -1;
    inputManager_ = nullptr;
    inputEventConsumer_ = nullptr;
}

void AccessibilityInputInterceptor::OnKeyEvent(MMI::KeyEvent &event)
{
    HILOG_DEBUG();

    event.AddFlag(MMI::InputEvent::EVENT_FLAG_NO_INTERCEPT);
    std::shared_ptr<MMI::KeyEvent> keyEvent = std::make_shared<MMI::KeyEvent>(event);
    if (inputManager_ != nullptr) {
        inputManager_->SimulateInputEvent(keyEvent);
    } else {
        HILOG_DEBUG("inputManager_ is null.");
    }
}

void AccessibilityInputInterceptor::OnPointerEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    HILOG_DEBUG("PointerAction is %{public}d.", event.GetPointerAction());
    HILOG_DEBUG("SourceType is %{public}d.", event.GetSourceType());
    HILOG_DEBUG("PointerId is %{public}d.", event.GetPointerId());

    event.AddFlag(MMI::InputEvent::EVENT_FLAG_NO_INTERCEPT);
    std::shared_ptr<MMI::PointerEvent> pointerEvent = std::make_shared<MMI::PointerEvent>(event);
    if (inputManager_ != nullptr) {
        inputManager_->SimulateInputEvent(pointerEvent);
    } else {
        HILOG_DEBUG("inputManager_ is null.");
    }
}

void AccessibilityInputInterceptor::SetAvailableFunctions(uint32_t availableFunctions)
{
    HILOG_DEBUG("function[%{public}d].", availableFunctions);

    if (availableFunctions_ == availableFunctions) {
        return;
    }
    availableFunctions_ = availableFunctions;
    DestroyTransmitters();
    CreateTransmitters();
}

void AccessibilityInputInterceptor::CreateTransmitters()
{
    HILOG_DEBUG("function[%{public}u].", availableFunctions_);

    if (!availableFunctions_) {
        DestroyInterceptor();
        return;
    }

    sptr<EventTransmission> header = nullptr;
    sptr<EventTransmission> current = nullptr;

    if (availableFunctions_& FEATURE_INJECT_TOUCH_EVENTS) {
        sptr<TouchEventInjector> touchEventInjector = new(std::nothrow) TouchEventInjector();
        if (!touchEventInjector) {
            HILOG_ERROR("touchEventInjector is null");
            return;
        }
        SetNextEventTransmitter(header, current, touchEventInjector);
        aams_->SetTouchEventInjector(touchEventInjector);
    }

    if (availableFunctions_& FEATURE_SCREEN_MAGNIFICATION) {
        sptr<AccessibilityZoomHandler> zoomHandler = new(std::nothrow) AccessibilityZoomHandler(0);
        if (!zoomHandler) {
            HILOG_ERROR("zoomHandler is null");
            return;
        }
        SetNextEventTransmitter(header, current, zoomHandler);
    }

    if (availableFunctions_& FEATURE_TOUCH_EXPLORATION) {
        sptr<TouchGuider> touchGuider = new(std::nothrow) TouchGuider();
        if (!touchGuider) {
            HILOG_ERROR("touchGuider is null");
            return;
        }
        touchGuider->StartUp();
        SetNextEventTransmitter(header, current, touchGuider);
    }

    SetNextEventTransmitter(header, current, instance_);
    pointerEventTransmitters_ = header;

    if (availableFunctions_& FEATURE_FILTER_KEY_EVENTS) {
        sptr<KeyEventFilter> keyEventFilter = new(std::nothrow) KeyEventFilter();
        if (!keyEventFilter) {
            HILOG_ERROR("keyEventFilter is null");
            return;
        }
        aams_->SetKeyEventFilter(keyEventFilter);
        keyEventFilter->SetNext(instance_);
        keyEventTransmitters_ = keyEventFilter;
    }

    CreateInterceptor();
}

void AccessibilityInputInterceptor::CreateInterceptor()
{
    HILOG_DEBUG();

    if (!inputManager_) {
        HILOG_DEBUG("inputManger is null.");
        return;
    }

    if (interceptorId_ == -1) {
        inputEventConsumer_ = std::make_shared<AccessibilityInputEventConsumer>();
        if ((availableFunctions_ & FEATURE_TOUCH_EXPLORATION) ||
            (availableFunctions_ & FEATURE_SCREEN_MAGNIFICATION)) {
            interceptorId_ = inputManager_->AddInterceptor(inputEventConsumer_);
            HILOG_DEBUG("interceptorId_ is %{public}d.", interceptorId_);
        }
    }

    if (keyEventInterceptorId_ == -1) {
        if (availableFunctions_ & FEATURE_FILTER_KEY_EVENTS) {
            keyEventInterceptorId_ = inputManager_->AddInterceptor(InterceptKeyEventCallback);
            HILOG_DEBUG("keyEventInterceptorId_ is %{public}d.", keyEventInterceptorId_);
        }
    }
}

void AccessibilityInputInterceptor::InterceptKeyEventCallback(std::shared_ptr<MMI::KeyEvent> keyEvent)
{
    HILOG_DEBUG(" start.");

    if (!instance_ || !instance_->eventHandler_) {
        HILOG_ERROR("eventHandler is nullptr.");
        return;
    }
    auto task = std::bind(&AccessibilityInputInterceptor::ProcessKeyEvent, instance_, keyEvent);
    instance_->eventHandler_->PostTask(task, AppExecFwk::EventQueue::Priority::LOW);
    HILOG_DEBUG(" end.");
}

void AccessibilityInputInterceptor::DestroyInterceptor()
{
    HILOG_DEBUG();

    if (!inputManager_) {
        HILOG_DEBUG("inputManager_ is null.");
        return;
    }
    if (keyEventInterceptorId_ != -1) {
        inputManager_->RemoveInterceptor(keyEventInterceptorId_);
        keyEventInterceptorId_ = -1;
    }

    if (interceptorId_ != -1) {
        inputManager_->RemoveInterceptor(interceptorId_);
        interceptorId_ = -1;
    }
}

void AccessibilityInputInterceptor::DestroyTransmitters()
{
    HILOG_DEBUG();

    if (pointerEventTransmitters_ != nullptr) {
        pointerEventTransmitters_->DestroyEvents();
        aams_->SetTouchEventInjector(nullptr);
        pointerEventTransmitters_= nullptr;
    }
    if (keyEventTransmitters_ != nullptr) {
        keyEventTransmitters_->DestroyEvents();
        aams_->SetKeyEventFilter(nullptr);
        keyEventTransmitters_ = nullptr;
    }
}

void AccessibilityInputInterceptor::NotifyAccessibilityEvent(AccessibilityEventInfo &event) const
{
    HILOG_DEBUG();

    if (pointerEventTransmitters_ != nullptr) {
        pointerEventTransmitters_->OnAccessibilityEvent(event);
    }
    if (keyEventTransmitters_ != nullptr) {
        keyEventTransmitters_->OnAccessibilityEvent(event);
    }
}

void AccessibilityInputInterceptor::ProcessPointerEvent(std::shared_ptr<MMI::PointerEvent> event) const
{
    HILOG_DEBUG();

    if (!pointerEventTransmitters_) {
        HILOG_DEBUG("pointerEventTransmitters_ is empty.");
        return;
    }

    pointerEventTransmitters_->OnPointerEvent(*event);
}

void AccessibilityInputInterceptor::ProcessKeyEvent(std::shared_ptr<MMI::KeyEvent> event) const
{
    HILOG_DEBUG();

    if (!keyEventTransmitters_) {
        HILOG_DEBUG("keyEventTransmitters_ is empty.");
        return;
    }

    keyEventTransmitters_->OnKeyEvent(*event);
}

void AccessibilityInputInterceptor::SetNextEventTransmitter(sptr<EventTransmission> &header,
    sptr<EventTransmission> &current, const sptr<EventTransmission> &next)
{
    HILOG_DEBUG();

    if (current != nullptr) {
        current->SetNext(next);
    } else {
        header = next;
    }
    current = next;
}

AccessibilityInputEventConsumer::AccessibilityInputEventConsumer()
{
    HILOG_DEBUG();

    auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
    if (aams != nullptr) {
        eventHandler_ = std::make_shared<AppExecFwk::EventHandler>(aams->GetMainRunner());
    }
}

AccessibilityInputEventConsumer::~AccessibilityInputEventConsumer()
{
    HILOG_DEBUG();

    eventHandler_ = nullptr;
}

void AccessibilityInputEventConsumer::OnInputEvent(std::shared_ptr<MMI::KeyEvent> keyEvent) const
{
    HILOG_DEBUG("OnInputEvent keyEvent start.");

    auto interceptor = AccessibilityInputInterceptor::GetInstance();
    if (!interceptor) {
        HILOG_DEBUG("interceptor is null.");
        return;
    }

    if (!eventHandler_) {
        HILOG_DEBUG("eventHandler_ is empty.");
        return;
    }

    auto task = std::bind(&AccessibilityInputInterceptor::ProcessKeyEvent, interceptor, keyEvent);
    eventHandler_->PostTask(task, AppExecFwk::EventQueue::Priority::LOW);
    HILOG_DEBUG("OnInputEvent keyEvent end.");
}

void AccessibilityInputEventConsumer::OnInputEvent(std::shared_ptr<MMI::PointerEvent> pointerEvent) const
{
    HILOG_DEBUG("OnInputEvent pointerEvent start.");
    HILOG_DEBUG("PointerAction is %{public}d.", pointerEvent->GetPointerAction());
    HILOG_DEBUG("SourceType is %{public}d.", pointerEvent->GetSourceType());
    HILOG_DEBUG("PointerId is %{public}d.", pointerEvent->GetPointerId());

    auto interceptor = AccessibilityInputInterceptor::GetInstance();
    if (!interceptor) {
        HILOG_DEBUG("interceptor is null.");
        return;
    }

    if (!eventHandler_) {
        HILOG_DEBUG("eventHandler_ is empty.");
        return;
    }
    auto task = std::bind(&AccessibilityInputInterceptor::ProcessPointerEvent, interceptor, pointerEvent);
    eventHandler_->PostTask(task, AppExecFwk::EventQueue::Priority::LOW);
    HILOG_DEBUG("OnInputEvent pointerEvent end.");
}
} // namespace Accessibility
} // namespace OHOS