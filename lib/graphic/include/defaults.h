// Copyright (c) 2020 Kieran Downie. All rights reserved.
//
// This file is part of insight.
//
// attitude is free software : you can redistribute it and /
// or modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// attitude is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with insight.  If not, see <https://www.gnu.org/licenses/>.
//
#ifndef GRAPHICS_DEFAULTS_H_
#define GRAPHICS_DEFAULTS_H_

#include <vector>

namespace insight {
namespace graphic {

using namespace std;

extern vector<int> kDefaultInactiveColor;
extern vector<vector<int>> kDefaultColorOrder;

}  // namespace graphic
}  // namespace insight
#endif // GRAPHICS_DEFAULTS_H_
