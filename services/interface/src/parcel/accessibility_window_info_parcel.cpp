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

#include "accessibility_window_info_parcel.h"
#include "accessibility_element_info_parcel.h"
#include "hilog_wrapper.h"
#include "parcel_util.h"

namespace OHOS {
namespace Accessibility {
AccessibilityWindowInfoParcel::AccessibilityWindowInfoParcel(const AccessibilityWindowInfo &accessibilityWindowInfo)
{
    HILOG_DEBUG("start.");
    AccessibilityWindowInfo *self = this;
    *self = accessibilityWindowInfo;
}

bool AccessibilityWindowInfoParcel::ReadFromParcel(Parcel &parcel)
{
    int32_t windowType = TYPE_WINDOW_INVALID;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowType);
    windowType_ = static_cast<WindowType>(windowType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowLayer_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, parentId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, windowTitle_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32Vector, parcel, &childIds_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, anchorId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, childNum_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, channelId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, active_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, focused_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, accessibilityFocused_);
    sptr<RectParcel> boundsInScreen = parcel.ReadStrongParcelable<RectParcel>();
    if (!boundsInScreen) {
        HILOG_ERROR("ReadStrongParcelable boundsInScreen failed.");
        return false;
    }
    boundsInScreen_ = *boundsInScreen;

    return true;
}

bool AccessibilityWindowInfoParcel::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int32_t>(windowType_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowLayer_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, parentId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, windowTitle_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32Vector, parcel, childIds_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, anchorId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, childNum_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, channelId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, active_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, focused_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, accessibilityFocused_);
    RectParcel rectParcel(boundsInScreen_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &rectParcel);

    return true;
};

sptr<AccessibilityWindowInfoParcel> AccessibilityWindowInfoParcel::Unmarshalling(Parcel &parcel)
{
    HILOG_DEBUG("start.");
    sptr<AccessibilityWindowInfoParcel> info = new AccessibilityWindowInfoParcel();
    if (!info->ReadFromParcel(parcel)) {
        HILOG_ERROR("ReadFromParcel failed.");
        info = nullptr;
        return nullptr;
    }
    return info;
}
} // namespace Accessibility
} // namespace OHOS