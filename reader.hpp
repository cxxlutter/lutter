#ifndef LUTTER_READER_HPP
#define LUTTER_READER_HPP

#include <iosfwd>

namespace lutter {

class scene;

bool read_pov_scene(std::istream& in, scene& s);

} // end namespace lutter

#endif /* LUTTER_READER_HPP */
