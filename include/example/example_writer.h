#ifndef LASER_EXAMPLE_EXAMPLE_WRITER_H
#define LASER_EXAMPLE_EXAMPLE_WRITER_H

#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

#include <util/grounding.h>

namespace laser {
namespace example {

class ExampleWriter {
  private:
    std::vector<std::shared_ptr<laser::util::Grounding>>
    remove_duplicates(std::vector<std::shared_ptr<laser::util::Grounding>>
                          input_groundings) const;

  public:
    ~ExampleWriter() = default;

    void write_output(std::string const &formatted_output_string) const;

    std::string
    format_output(uint64_t time,
                  std::vector<std::shared_ptr<laser::util::Grounding>>
                      output_vector) const;
};

} // namespace example 
} // namespace laser

#endif // LASER_EXAMPLE_EXAMPLE_WRITER_H
