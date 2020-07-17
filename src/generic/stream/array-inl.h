namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace stream {

really_inline array::array() noexcept : value{}, finished{}, error{} {}
really_inline array::array(stream::json *json, uint32_t depth, bool _finished, error_code _error) noexcept : value(json, depth), finished{_finished}, error{_error} {}
really_inline array::array(stream::value &parent, error_code _error) noexcept : array(parent.json, parent.depth+1, true, _error) {}
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::stream::value&> array::operator*() noexcept { return { value, error }; }
really_inline bool array::operator!=(array &) noexcept { return !finished; }
really_inline array &array::operator++() noexcept { advance(); return *this; }

really_inline void array::advance() noexcept {
  // We should never get here we are finished or if there is an error! The user is supposed to stop
  // on error, and != stops on finished.
  SIMDJSON_ASSUME(!finished && !error);

  // Finish iterating any partly-iterated child arrays/objects
  value.finish();

  // Check whether we are finished
  // TODO We safeguard against the user using the library incorrectly (not breaking out of loops
  // on error) here using !has_next, but that might confuse the optimizer, so check performance
  // without it (and maybe wrap in if debug?).
  finished = value.json->advance_if_end(']');
  if (finished) { logger::log_end_event("array", value.json); }

  // Jump past the comma
  bool has_next = value.json->advance_if(',');
  if (!finished && !has_next) { logger::log_error("missing comma", value.json); }
  error = (!finished && !has_next) ? TAPE_ERROR : SUCCESS;
}

really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::stream::array> array::try_begin(stream::value &parent) noexcept {
  if (!parent.json->advance_if_start('[')) {
    logger::log_error("not an array", parent.json);
    return simdjson_result<SIMDJSON_IMPLEMENTATION::stream::array>(parent, INCORRECT_TYPE);
  }
  return begin(parent, true);
}

really_inline array array::begin(stream::value &parent, error_code error) noexcept {
  return begin(parent, parent.json->advance_if_start('['), error);
}

really_inline array array::begin(stream::value &parent, bool is_array, error_code error) noexcept {
  SIMDJSON_ASSUME(!is_array || parent.json->depth == parent.depth+1);
  logger::log_start_event("array", parent.json);
  if (!is_array) { logger::log_error("not an array", parent.json); }
  error = error ? error : (is_array ? SUCCESS : INCORRECT_TYPE);

  // Check for []
  bool finished = !error && parent.json->advance_if(']');
  if (finished) { logger::log_end_event("empty array", parent.json); }
  else { logger::log_event("first element", parent.json); }

  return { parent.json, parent.depth+1, finished, error };
}

really_inline array array::end() noexcept {
  return {};
}

} // namespace stream
} // namespace SIMDJSON_IMPLEMENTATION

namespace {
  using namespace simdjson::SIMDJSON_IMPLEMENTATION::stream;
}

really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::stream::array>::simdjson_result(SIMDJSON_IMPLEMENTATION::stream::array &&value) noexcept
    : internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::stream::array>(std::forward<SIMDJSON_IMPLEMENTATION::stream::array>(value)) {}
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::stream::array>::simdjson_result(SIMDJSON_IMPLEMENTATION::stream::value &parent, error_code error) noexcept
    : internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::stream::array>({ parent, error }, error) {}

really_inline SIMDJSON_IMPLEMENTATION::stream::array simdjson_result<SIMDJSON_IMPLEMENTATION::stream::array>::begin() noexcept {
  return first;
}
really_inline SIMDJSON_IMPLEMENTATION::stream::array simdjson_result<SIMDJSON_IMPLEMENTATION::stream::array>::end() noexcept {
  return {};
}

} // namespace simdjson
