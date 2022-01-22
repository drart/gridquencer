"use strict";

var fluid = require("infusion");
require("infusion-electron");
require("./gridquencer-app.js");

var gridquencer = fluid.registerNamespace("gridquencer");
gridquencer.app();
