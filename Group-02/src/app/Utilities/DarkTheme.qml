pragma Singleton
import QtQuick

QtObject {
    readonly property Palette palette: Palette
    {
        alternateBase: "#000"
        base: "#000"
        button: "#111"
        buttonText: "#fff"
        dark: "#666"
        highlight: "#d73"
        highlightedText: "#000"
        light: "#000"
        mid: "#444"
        midlight: "#333"
        placeholderText: "#80000000"
        shadow: "#888"
        text: "#fff"
        window: "#222"
        windowText: "#fff"
    }
}
