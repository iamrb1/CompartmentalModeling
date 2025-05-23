pragma Singleton

import QtQuick

QtObject {
    readonly property Palette palette: Palette
    {
        alternateBase: "#fff"
        base: "#fff"
        button: "#eee"
        buttonText: "#000"
        dark: "#999"
        highlight: "#38c"
        highlightedText: "#fff"
        light: "#fff"
        mid: "#bbb"
        midlight: "#ccc"
        placeholderText: "#80000000"
        shadow: "#777"
        text: "#000"
        window: "#eee"
        windowText: "#000"
        property color danger: "#ff1919"
        property color info: "#3498db"
        property color warning: "#f39c12"
    }
}
