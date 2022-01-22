"use strict";

var fluid = require("infusion");

fluid.defaults("gridquencer.app", {
    gradeNames: "electron.app",
    model: {
        commandLineSwitches: {
            expander: {
                funcName: "fluid.stringTemplate",
                args: [
                    "autoplay-policy", 
                    "no-user-gesture-required"
                ]
            }
        }
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
                        contextIsolation: false,
                        enableRemoteModule: true
                    },
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
            },
        },
    },

});


