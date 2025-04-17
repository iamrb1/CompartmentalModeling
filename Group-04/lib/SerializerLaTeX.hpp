/**
 * @file SerializerLaTeX.hpp
 * @author Krish
 * @brief Serializer for exporting RichText as LaTeX
 */
#pragma once

#include <concepts>
#include <format>
#include <string>

#include "RichText.hpp"
#include "SerializerCommon.hpp"

namespace cse {

// Generate a LaTeX serializer for the given character type and underlying string-like
template <typename CharT = char, typename Underlying = std::basic_string<CharT>>
  requires std::derived_from<Underlying, std::basic_string<CharT>>
auto SerializerLaTeX() {
  struct BasicRichText<CharT, Underlying>::Serializer out{"LaTeX"};

  // Document wrapper
  out.header = str_to_underlying<CharT, Underlying>(
    R"(\documentclass{article}
\usepackage{hyperref}
\usepackage[normalem]{ulem}
\begin{document}
)"
  );
  out.footer = str_to_underlying<CharT, Underlying>(
    R"(
\end{document}
)"
  );

  // Bold
  out.AddRule(
    "bold",
    str_to_underlying<CharT, Underlying>("\\textbf{"),
    str_to_underlying<CharT, Underlying>("}")
  );

  // Italic
  out.AddRule(
    "italic",
    str_to_underlying<CharT, Underlying>("\\textit{"),
    str_to_underlying<CharT, Underlying>("}")
  );

  // Strikethrough
  out.AddRule(
    "strikethrough",
    str_to_underlying<CharT, Underlying>("\\sout{"),
    str_to_underlying<CharT, Underlying>("}")
  );

  // **Fixed** link rule: one leading backslash
  out.AddRule(
    "link",
    [](const TextFormat& f) -> Underlying {
      cse_assert(std::holds_alternative<std::string>(f.metadata));
      const auto& url = std::get<std::string>(f.metadata);
      // std::format: "\\href{{{}}}{{" -> \href{url}{
      return str_to_underlying<CharT, Underlying>(
        std::format("\\href{{{}}}{{", url)
      );
    },
    str_to_underlying<CharT, Underlying>("}")
  );

  // Headers
  out.AddRule(
    "header",
    [](const TextFormat& f) -> Underlying {
      cse_assert(std::holds_alternative<int32_t>(f.metadata));
      int32_t level = std::get<int32_t>(f.metadata);
      std::string cmd;
      switch (level) {
        case 1: cmd = "\\section{";    break;
        case 2: cmd = "\\subsection{"; break;
        case 3: cmd = "\\subsubsection{"; break;
        default: cmd = "\\paragraph{";  break;
      }
      return str_to_underlying<CharT, Underlying>(cmd);
    },
    str_to_underlying<CharT, Underlying>("}\n")
  );

  return out;
}

}  // namespace cse
