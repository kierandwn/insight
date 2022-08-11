// Copyright (c) 2020 Kieran Downie. All rights reserved.
//
// This file is part of insight.
//
// insight is free software : you can redistribute it and /
// or modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// insight is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with insight.  If not, see <https://www.gnu.org/licenses/>.
//
#ifndef DISPLAY_GROUP_H
#define DISPLAY_GROUP_H

#include "ApplicationInterface.h"

#include <vector>


namespace insight {
namespace layout {

class InsightDisplayGroup
{
protected:
    std::vector<graphic::ApplicationInterface *> grouped_object_refs_;
    bool does_group_update_together_ = false;

public:
    InsightDisplayGroup();

    void updateGroupCursorPositions(double);
    void updateGroupViewLimits(double, double);
};

} // namespace layout
} // namesspace insight

#endif // DISPLAY_GROUP_H
