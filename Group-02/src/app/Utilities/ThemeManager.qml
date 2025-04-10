pragma Singleton
import QtQuick
import Utilities
import QtCore

QtObject {
    id: root
    // Current theme
    property var theme: DarkTheme

    // // Set theme by name
    // function setTheme(themeName) {
    //     if (themeName === "dark") {
    //         theme = DarkTheme
    //         Settings.setValue("theme", "dark")
    //     } else {
    //         theme = LightTheme
    //         Settings.setValue("theme", "light")
    //     }
    //     settings.theme = themeName
    // }
    //
    // // Load theme from settings
    // function loadTheme() {
    //     var themeName = Settings.value("theme", "light")
    //     setTheme(themeName)
    // }
    //
    // // Toggle between light/dark
    // function toggleTheme() {
    //     var themeName = Settings.value("theme", "light")
    //     setTheme(themeName === "dark" ? "light" : "dark")
    // }
    //
    // // Initialize
    // Component.onCompleted: loadTheme()
}
