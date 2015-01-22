// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUTTER_READER_HPP
#define LUTTER_READER_HPP

#include <iosfwd>

namespace lutter {

class scene;

bool read_pov_scene(std::istream& in, scene& s);

} // end namespace lutter

#endif /* LUTTER_READER_HPP */
