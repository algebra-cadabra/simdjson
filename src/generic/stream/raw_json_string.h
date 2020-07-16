#include "simdjson/error.h"

namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace stream {

/**
 * A string escaped per JSON rules, terminated with quote (")
 *
 * (In other words, a pointer to the beginning of a string, just after the start quote, inside a
 * JSON file.)
 */
class raw_json_string {
public:
  really_inline raw_json_string() noexcept;
  really_inline raw_json_string(const uint8_t * _buf) noexcept;
  really_inline const char * raw() const noexcept;
  really_inline WARN_UNUSED simdjson_result<std::string_view> unescape(uint8_t *&dst) const noexcept;
private:
  const uint8_t * const buf;
};

really_inline bool operator==(const raw_json_string &a, std::string_view b) noexcept;
really_inline bool operator==(std::string_view a, const raw_json_string &b) noexcept;

} // namespace stream
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace simdjson
