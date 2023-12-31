#ifndef SRC_LOGGER_HPP_
#define SRC_LOGGER_HPP_

#include <string>

namespace logger {

  enum level { level_debug, level_info, level_warn, level_error };

  void set_level(level log_level);

  level get_level();

  /**
   * Log debug message.
   */
  void debug(std::string format, ...);

  /**
   * Log warning message.
   */
  void warn(std::string format, ...);

  /**
   * Log error message.
   */
  void error(std::string format, ...);

  /**
   * Log information message.
   */
  void info(std::string format, ...);

  /**
   * Log error provided by errno from last system call.
   */
  void last(std::string format, ...);

}  // namespace logger

#endif  // SRC_LOGGER_HPP_