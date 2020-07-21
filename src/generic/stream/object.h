#include "simdjson/error.h"

namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace stream {

/**
 * A forward-only JSON object.
 */
class object {
public:
  really_inline object() noexcept;

  really_inline simdjson_result<value&> operator[](std::string_view key) noexcept;

  //
  // iterator interface
  //
  really_inline simdjson_result<stream::field&> operator*() noexcept;
  really_inline bool operator==(const object &other) noexcept;
  really_inline bool operator!=(const object &other) noexcept;
  really_inline object &operator++() noexcept;

  really_inline object begin() noexcept;
  really_inline object end() noexcept;

protected:
  /** Create a valid object */
  really_inline object(stream::value &parent, const uint8_t *key_string) noexcept;
  /** Create an error object */
  really_inline object(stream::value &parent, error_code _error) noexcept;

  really_inline void advance() noexcept;
  static really_inline simdjson_result<stream::object> try_begin(stream::value &parent) noexcept;
  static really_inline object begin(stream::value &parent) noexcept;

  stream::field field;
  // TODO check whether this gets cleverly elided by the compiler or not. If not, store it in the json
  // so we don't keep multiple of them
  error_code error;

  friend class value;
  friend class json;
  friend class simdjson_result<stream::object>;
};

} // namespace stream
} // namespace SIMDJSON_IMPLEMENTATION

template<>
struct simdjson_result<SIMDJSON_IMPLEMENTATION::stream::object> : public internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::stream::object> {
public:
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::stream::object &&value) noexcept; ///< @private
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::stream::value &parent, error_code error) noexcept; ///< @private

  really_inline SIMDJSON_IMPLEMENTATION::stream::object begin() noexcept;
  really_inline SIMDJSON_IMPLEMENTATION::stream::object end() noexcept;
  really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::stream::value&> operator[](std::string_view key) noexcept;
};

} // namespace simdjson
