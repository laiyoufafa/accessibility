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

#ifndef ACCESSIBILITY_TOUCH_GUIDER_H
#define ACCESSIBILITY_TOUCH_GUIDER_H

#include <string>
#include "accessibility_element_info.h"
#include "accessibility_element_operator_callback_stub.h"
#include "accessibility_event_transmission.h"
#include "accessibility_gesture_recognizer.h"
#include "accessible_ability_manager_service.h"

namespace OHOS {
namespace Accessibility {
class TouchGuider;

const int32_t MAX_POINTER_COUNT = 32;
const int64_t EXIT_GESTURE_REC_TIMEOUT = 2000; // millisecond
const double MAX_DRAG_GESTURE_COSINE = 0.525321989;
const int32_t MINI_POINTER_DISTANCE_DIP = 200;
const int32_t INDEX_0 = 0;
const int32_t INDEX_1 = 1;
const int32_t INIT_POINT_ID = -1;
const float INIT_MMIPOINT = 0.0f;
#define DIVIDE_2(num) ((num) / 2)
#define EPSINON 0.01

/**
 * @brief touch Guider state define
 */
enum class TouchGuideState : int32_t {
    TOUCH_GUIDING,
    DRAGGING,
    TRANSMITTING,
    GESTURE_RECOGNIZING
};

/**
 * @brief Click location define
 */
enum ClickLocation : int32_t {
    CLICK_NONE,
    CLICK_ACCESSIBILITY_FOCUS,
    CLICK_LAST_TOUCH_GUIDE
};

/**
 * @brief struct to record injected pointers.
 */
struct InjectedEventRecorder {
    int32_t downPointers;
    int32_t downPointerNum;
    int64_t lastDownTime;
    std::shared_ptr<MMI::PointerEvent> lastHoverEvent;
};

/**
 * @brief struct to record received pointers.
 */
struct ReceivedEventRecorder {
    int32_t pointerDownX[MAX_POINTER_COUNT];
    int32_t pointerDownY[MAX_POINTER_COUNT];
    std::shared_ptr<MMI::PointerEvent> lastEvent;
};

enum ChangeAction : int32_t {
    NO_CHANGE,
    HOVER_MOVE,
    POINTER_DOWN,
    POINTER_UP,
    POINTER_MOVE,
};

class TGEventHandler : public AppExecFwk::EventHandler {
public:
    TGEventHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner,
                 TouchGuider &tgServer);
    virtual ~TGEventHandler() = default;
    /**
     * @brief Process the event of install system bundles.
     * @param event Indicates the event to be processed.
     * @return
     */
    virtual void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) override;

private:
    /**
     * @brief Send HoverEnter and HoverMove to Multimodal.
     * @param
     * @return
     */
    void HoverEnterAndMoveRunner();

    /**
     * @brief Send HoverExit to Multimodal.
     * @param
     * @return
     */
    void HoverExitRunner();
    TouchGuider &tgServer_;
};

class TouchGuider : public EventTransmission {
public:
    /**
     * @brief A constructor used to create a touchGuide instance.
     * @param
     * @return
     */
    TouchGuider();

    /**
     * @brief A destructor used to delete the touchGuide instance.
     * @param
     * @return
     */
    ~TouchGuider() {}

    /**
     * @brief TouchGuide start up.
     * @param
     * @return
     */
    void StartUp();

    /**
     * @brief Handle pointer events from previous event stream node.
     *
     * @param event  the pointer event to be handled.
     * @return
     */
    void OnPointerEvent(MMI::PointerEvent &event) override;

    /**
     * @brief Handle accessibility events from previous event stream node.
     *
     * @param event  the accessibility event  to be handled.
     * @return
     */
    void OnAccessibilityEvent(AccessibilityEventInfo &event) override;

    /**
     * @brief Clear event state from specific input source.
     * @param inputSource  the input source.
     * @return
     */
    void ClearEvents(uint32_t inputSource) override;

    /**
     * @brief Destroy event state.
     * @param
     * @return
     */
    void DestroyEvents() override;

    /**
     * @brief Send event to multimodal input.
     * @param event the event prepared to send to Multimodal
     * @param action the action of the event
     * @return
     */
    void SendEventToMultimodal(MMI::PointerEvent &event, int32_t action);

    /**
     * @brief Send accessibility event to specific AccessiblityAbility.
     * @param eventType the type of the event
     * @return
     */
    void SendAccessibilityEventToAA(EventType eventType);

    /**
     * @brief Get hover enter and move event.
     * @param
     * @return Returns pointerEvents_ list.
     */
    std::list<MMI::PointerEvent> getHoverEnterAndMoveEvent();

    /**
     * @brief Clear hover enter and move event.
     * @param
     * @return
     */
    void ClearHoverEnterAndMoveEvent();

    /**
     * @brief Get last received event.
     * @param
     * @return Returns last event ptr.
     */
    std::shared_ptr<MMI::PointerEvent> getLastReceivedEvent();

    static constexpr uint32_t EXIT_GESTURE_REC_MSG = 0;
    static constexpr uint32_t SEND_HOVER_ENTER_MOVE_MSG = 1;
    static constexpr uint32_t SEND_HOVER_EXIT_MSG = 2;
    static constexpr uint32_t SEND_TOUCH_INTERACTION_END_MSG = 3;
    static constexpr uint32_t SEND_TOUCH_GUIDE_END_MSG = 4;

     /* For TouchGuide */
    inline void OnTouchInteractionStart()
    {
        isTouchStart_ = true;
    }

    inline void OnTouchInteractionEnd()
    {
        isTouchStart_ = false;
    }

    /**
     * @brief Perform action on Accessibility Focus.
     * @param action the action of Accessibility node.
     * @return Returns true if the action perform successfully; returns false code otherwise.
     */
    bool ExecuteActionOnAccessibilityFocused(const ActionType &action);

private:
    class TouchGuideListener : public AccessibilityGestureRecognizeListener {
    public:
        /**
         * @brief A constructor used to create a TouchGuideListener instance.
         * @param
         * @return
         */
        explicit TouchGuideListener(TouchGuider &server) : server_(server) {};

        /**
         * @brief Prepare to send the event corresponding to the long press to the Multimodal.
         * @param event the touch event from Multimodal
         * @return
         */
        void OnDoubleTapLongPress(MMI::PointerEvent &event) override;

        /**
         * @brief Prepare to send the event corresponding to the single tap to the Multimodal.
         * @param event the touch event from Multimodal
         * @return
         */
        bool OnDoubleTap(MMI::PointerEvent &event) override;

        /**
         * @brief Send GESTURE_BEGIN to AccessiblityAbility.
         * @param
         * @return
         */
        bool OnStarted() override;

        /**
         * @brief Send GESTURE_END and TOUCH_END to AccessiblityAbility.
         * @param gestureId the id of gesture
         * @return
         */
        bool OnCompleted(GestureType gestureId) override;

        /**
         * @brief The gesture has been cancelled.
         * @param event the touch event from Multimodal
         * @return
         */
        bool OnCancelled(MMI::PointerEvent &event) override;

    private:
        /**
         * @brief Send the single tap events to the Multimodal.
         * @param event the event prepared to send to Multimodal
         * @param point the click point
         * @return whether the message is successfully sent.
         */
        bool TransformToSingleTap(MMI::PointerEvent &event, MMI::PointerEvent::PointerItem &point);
        TouchGuider &server_;
    };

    class ElementOperatorCallbackImpl : public AccessibilityElementOperatorCallbackStub {
    public:
        ElementOperatorCallbackImpl() = default;
        ~ElementOperatorCallbackImpl() = default;

        virtual void SetSearchElementInfoByAccessibilityIdResult(const std::vector<AccessibilityElementInfo> &infos,
            const int32_t requestId) override;
        virtual void SetSearchElementInfoByTextResult(const std::vector<AccessibilityElementInfo> &infos,
            const int32_t requestId) override;
        virtual void SetFindFocusedElementInfoResult(const AccessibilityElementInfo &info,
            const int32_t requestId) override;
        virtual void SetFocusMoveSearchResult(const AccessibilityElementInfo &info, const int32_t requestId) override;
        virtual void SetExecuteActionResult(const bool succeeded, const int32_t requestId) override;

    private:
        std::promise<void> promise_;
        bool executeActionResult_ = false;
        AccessibilityElementInfo accessibilityInfoResult_ = {};
        std::vector<AccessibilityElementInfo> elementInfosResult_;

        friend class TouchGuider;
    };

    /**
     * @brief Determine whether to clear the touchguide.
     * @param
     * @return
     */
    void Clear();

    /**
     * @brief clear the touchguide.
     * @param event the last event from Multimodal
     * @return
     */
    void Clear(MMI::PointerEvent &event);

    /**
     * @brief Handle touch events on touchExploring state.
     * @param event the touch event from Multimodal
     * @return
     */
    void HandleTouchGuidingState(MMI::PointerEvent &event);

    /**
     * @brief Handle touch events on dragging state.
     * @param event the touch event from Multimodal
     * @return
     */
    void HandleDraggingState(MMI::PointerEvent &event);

    /**
     * @brief Handle touch events on transmitting state.
     * @param event the touch event from Multimodal
     * @return
     */
    void HandleTransmitingState(MMI::PointerEvent &event);

    /**
     * @brief Determine whether it is a drag gesture.
     * @param event the touch event from Multimodal
     * @return whether the dragGesture is accepted.
     */
    bool IsDragGestureAccept(MMI::PointerEvent &event);

    /**
     * @brief Get Angle Cos value.
     * @param offsetX the X value
     * @param offsetY the Y value
     * @param isGetX whether is the Angle corresponding to the X axis
     * @return Angle Cos value.
     */
    float GetAngleCos(float offsetX, float offsetY, bool isGetX);

    /**
     * @brief Get the info of injected event.
     * @param event the event prepared to send to Multimodal
     * @return
     */
    void RecordInjectedEvent(MMI::PointerEvent &event);

    /**
     * @brief Get the info of Received event.
     * @param event event the touch event from Multimodal
     * @return
     */
    void RecordReceivedEvent(MMI::PointerEvent &event);

    /**
     * @brief Clear received recorder info.
     * @param
     * @return
     */
    void ClearReceivedEventRecorder();

    /**
     * @brief Clear Injected recorder info.
     * @param
     * @return
     */
    void ClearInjectedEventRecorder();

    /**
     * @brief Send exit event to multimodal.
     * @param
     * @return
     */
    void SendExitEvents();

    /**
     * @brief Send all down events to multimodal.
     * @param event the event prepared to send to Multimodal
     * @return
     */
    void SendAllDownEvents(MMI::PointerEvent &event);

    /**
     * @brief Send all up events to multimodal.
     * @param event the event prepared to send to Multimodal
     * @return
     */
    void SendUpForAllInjectedEvent(MMI::PointerEvent &event);

    /**
     * @brief Send exit message.
     * @param
     * @return
     */
    void PostGestureRecognizeExit();

    /**
     * @brief Send enter and move message.
     * @param event event the touch event from Multimodal
     * @return
     */
    void PostHoverEnterAndMove(MMI::PointerEvent &event);

    /**
     * @brief Send exit message.
     * @param
     * @return
     */
    void PostHoverExit();

    /**
     * @brief Send accessibility event message.
     * @param innerEventID the id of inner event
     * @return
     */
    void PostAccessibilityEvent(uint32_t innerEventID);

    /**
     * @brief Cancel message.
     * @param innerEventID the id of inner event
     * @return
     */
    void CancelPostEvent(uint32_t innerEventID);

    /**
     * @brief Cancel message if it has been sent.
     * @param innerEventID the id of inner event
     * @return
     */
    void CancelPostEventIfNeed(uint32_t innerEventID);

    /**
     * @brief Check whether it has been sending.
     * @param innerEventID the id of inner event
     * @return
     */
    bool HasEventPending(uint32_t innerEventID);

    /**
     * @brief Force send and remove event.
     * @param innerEventID the id of inner event
     * @param event event the touch event from Multimodal
     * @return
     */
    void ForceSendAndRemoveEvent(uint32_t innerEventID, MMI::PointerEvent &event);

    /**
     * @brief Handle down events on touchExploring state.
     * @param event event the touch event from Multimodal
     * @return
     */
    void HandleTouchGuidingStateInnerDown(MMI::PointerEvent &event);

    /**
     * @brief Handle move events on touchExploring state.
     * @param event event the touch event from Multimodal
     * @return
     */
    void HandleTouchGuidingStateInnerMove(MMI::PointerEvent &event);

    /**
     * @brief Handle move events on dragging state.
     * @param event event the touch event from Multimodal
     * @return
     */
    void HandleDraggingStateInnerMove(MMI::PointerEvent &event);

    int32_t currentState_ = -1;
    int32_t longPressPointId_ = INIT_POINT_ID;
    float longPressOffsetX_ = INIT_MMIPOINT;
    float longPressOffsetY_ = INIT_MMIPOINT;
    bool isTouchStart_ = false;
    bool isTouchGuiding_ = false;
    ReceivedEventRecorder receivedRecorder_ = {};
    InjectedEventRecorder injectedRecorder_ = {};
    std::list<MMI::PointerEvent> pointerEvents_ {};
    AccessibilityGestureRecognizer gestureRecognizer_;
    std::unique_ptr<TouchGuideListener> touchGuideListener_ = nullptr;
    std::shared_ptr<AccessibleAbilityManagerService> pAams_ = nullptr;
    std::shared_ptr<TGEventHandler> handler_ = nullptr;
    std::shared_ptr<AppExecFwk::EventRunner> runner_ = nullptr;
};
} // namespace Accessibility
} // namespace OHOS
#endif  // ACCESSIBILITY_TOUCH_GUIDER_H