// app/web/Bindings.hpp
/**
 * @file Bindings.hpp
 * @brief Emscripten bindings for RichTextState, disambiguating overloads.
 */
#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <emscripten/bind.h>
#include "RichTextState.hpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(rich_text_module) {
  class_<RichTextState>("RichTextState")
    .constructor<>()

    // Text editing
    .function("edit_start_pos",   &RichTextState::edit_start_pos)
    .function("edit_start_range", &RichTextState::edit_start_range)
    .function("edit_change",      &RichTextState::edit_change)
    .function("setText",          &RichTextState::setText)
    .function("getText",          &RichTextState::getText)

    // Single‑arg formatting
    .function("applyBold",          
      static_cast<void(RichTextState::*)()>(&RichTextState::applyBold))
    .function("applyItalic",        
      static_cast<void(RichTextState::*)()>(&RichTextState::applyItalic))
    .function("applyStrikethrough", 
      static_cast<void(RichTextState::*)()>(&RichTextState::applyStrikethrough))
    .function("applyColor",         
      static_cast<void(RichTextState::*)(const std::string&)>(&RichTextState::applyColor))
    .function("applyLink",          
      static_cast<void(RichTextState::*)(const std::string&)>(&RichTextState::applyLink))
    .function("applyHeader",        
      static_cast<void(RichTextState::*)(int)>(&RichTextState::applyHeader))

    // Range‑based formatting (distinct JS names)
    .function("applyBoldRange",      
      static_cast<void(RichTextState::*)(std::size_t, std::size_t)>(&RichTextState::applyBold))
    .function("applyItalicRange",    
      static_cast<void(RichTextState::*)(std::size_t, std::size_t)>(&RichTextState::applyItalic))
    .function("applyStrikeRange",    
      static_cast<void(RichTextState::*)(std::size_t, std::size_t)>(&RichTextState::applyStrikethrough))
    .function("applyColorRange",     
      static_cast<void(RichTextState::*)(const std::string&, std::size_t, std::size_t)>(&RichTextState::applyColor))
    .function("applyLinkRange",      
      static_cast<void(RichTextState::*)(const std::string&, std::size_t, std::size_t)>(&RichTextState::applyLink))
    .function("applyHeaderRange",    
      static_cast<void(RichTextState::*)(int, std::size_t, std::size_t)>(&RichTextState::applyHeader))

    // Output serialization
    .function("output_formats", &RichTextState::output_formats)
    .function("output",         &RichTextState::output)
    .function("getHTML",        &RichTextState::getHTML)
    .function("getRawHTML",     &RichTextState::getRawHTML)
    .function("getMarkdown",    &RichTextState::getMarkdown)
    .function("getRTF",         &RichTextState::getRTF)
    .function("getLaTeX",       &RichTextState::getLaTeX)

    // Undo / Redo
    .function("undo", &RichTextState::undo)
    .function("redo", &RichTextState::redo)
    ;

  // Expose vectors
  register_vector<std::vector<std::string>>("VVString");
  register_vector<std::string>("VString");
}
