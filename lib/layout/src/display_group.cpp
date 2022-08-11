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

#include "display_group.h"
#include "ApplicationInterface.h"

namespace insight {
namespace layout {

void InsightDisplayGroup::updateGroupCursorPositions(double selected_value_indepvar)
{
    if (!does_group_update_together_) return;

    std::vector<graphic::ApplicationInterface *>::iterator itr = grouped_object_refs_.begin();
    for (itr = grouped_object_refs_.begin(); itr != grouped_object_refs_.end(); ++itr)
    {
        ApplicationInterface * obj = *(itr);
        obj->update_cursor_position(selected_value_indepvar);
    }

}

void InsightDisplayGroup::updateGroupViewLimits(double low_bound_time, double high_bound_time)
{

}

} // namespace layout
} // namesspace insight
