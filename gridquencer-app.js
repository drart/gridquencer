"use strict";

var fluid = require("infusion"),
    gridquencer = fluid.registerNamespace("gridquencer");

fluid.defaults("gridquencer.app", {
    gradeNames: "electron.app",

    commandLineSwitches: {
        "autoplay-policy": null,
        "no-user-gesture-required": null
    },

    components: {
        mainWindow: {
            createOnEvent: "onReady",
            type: "electron.browserWindow",
            options: {
                windowOptions: {
                    title: "Gridquencer",
                    width: 1080,
                    height: 480,
                    x: 100,
                    y: 100,
                    webPreferences: {
                        nodeIntegration: true,
                        contextIsolation: false
                    }
                },
                model: {
                    url: {
                        expander: {
                            funcName: "fluid.stringTemplate",
                            args: [
                                "%url/main-window.html",
                                "{app}.env.appRoot"
                            ]
                        }
                    }
                }
            }
        }
    },

    listeners: {
        "onCreate.disableProcessReuse": {
            funcName: "gridquencer.app.disableProcessReuse",
            args: ["{that}.app"]
        }
    }
});

gridquencer.app.disableProcessReuse = function (electronApp) {
    electronApp.allowRendererProcessReuse = false;
};
