pragma Singleton

import QtQuick
import QtQml.Models

QtObject {
    id: themeManager
    // List of available themes
    readonly property var themeList: [{
        "name": "Light",
        "value": "light",
        "palette": LightTheme.palette
    }, {
        "name": "Dark",
        "value": "dark",
        "palette": DarkTheme.palette
    }]

    // Current theme
    property string theme: "light"
    property var palette: LightTheme.palette

    // Function to change the theme
    function setTheme(themeName) {
        themeList.forEach(item => {
            if (item.value === themeName) {
                theme = themeName
                palette = item.palette
            }
        })
    }
}
