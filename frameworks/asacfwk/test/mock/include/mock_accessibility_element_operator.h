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

#ifndef MOCK_ACCESSIBILITY_ELEMENT_OPERATOR_H
#define MOCK_ACCESSIBILITY_ELEMENT_OPERATOR_H

#include <gmock/gmock.h>
#include "accessibility_element_operator.h"

namespace OHOS {
namespace Accessibility {
class MockAccessibilityElementOperator : public AccessibilityElementOperator {
public:
    MockAccessibilityElementOperator() = default;
    virtual ~MockAccessibilityElementOperator() = default;
    MOCK_METHOD4(
        SearchElementInfoByAccessibilityId, void(const long elementId, const int requestId,
                                                AccessibilityElementOperatorCallback& callback, const int mode));
    MOCK_METHOD4(SearchElementInfosByText, void(const long elementId, const std::string& text, const int requestId,
                                               AccessibilityElementOperatorCallback& callback));
    MOCK_METHOD4(FindFocusedElementInfo, void(const long elementId, const int focusType, const int requestId,
                                             AccessibilityElementOperatorCallback& callback));
    MOCK_METHOD4(FocusMoveSearch, void(const long elementId, const int direction, const int requestId,
                                      AccessibilityElementOperatorCallback& callback));
    MOCK_METHOD5(ExecuteAction,
        void(const long elementId, const int action, const std::map<std::string, std::string> actionArguments,
            const int requestId, AccessibilityElementOperatorCallback& callback));
    MOCK_METHOD0(OutsideTouch, void());
    MOCK_METHOD0(ClearFocus, void());
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_ACCESSIBILITY_ELEMENT_OPERATOR_H