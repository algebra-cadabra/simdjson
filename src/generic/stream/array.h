#include "simdjson/error.h"

namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace stream {

/**
 * A forward-only JSON array.
 */
class array {
public:
  really_inline array() noexcept;

  //
  // Iterator interface
  //
  really_inline simdjson_result<stream::value&> operator*() noexcept;
  really_inline bool operator!=(array &other) noexcept;
  really_inline array &operator++() noexcept;
  really_inline array begin() noexcept;
  really_inline array end() noexcept;

protected:
  really_inline array(stream::json *_json, uint32_t _depth, bool _finished, error_code _error=SUCCESS) noexcept;
  really_inline array(stream::value &parent, error_code _error) noexcept;

  really_inline void advance() noexcept;
  really_inline static simdjson_result<array> try_begin(stream::value &parent) noexcept;
  really_inline static array begin(stream::value &parent, error_code error = SUCCESS) noexcept;
  really_inline static array begin(stream::value &parent, bool is_array, error_code error = SUCCESS) noexcept;

  stream::value value;
  bool finished;
  error_code error;

  friend class stream::value;
  friend class simdjson_result<stream::value&>;
  friend class simdjson_result<stream::array>;
};

} // namespace stream
} // namespace SIMDJSON_IMPLEMENTATION

namespace {
  using namespace simdjson::SIMDJSON_IMPLEMENTATION::stream;
}

template<>
struct simdjson_result<array> : public internal::simdjson_result_base<array> {
public:
  really_inline simdjson_result(array &&value) noexcept; ///< @private
  really_inline simdjson_result(stream::value &parent, error_code error) noexcept; ///< @private

  really_inline simdjson_result<stream::value&> operator[](std::string_view key) noexcept;

  really_inline array begin() noexcept;
  really_inline array end() noexcept;
};

} // namespace simdjson
