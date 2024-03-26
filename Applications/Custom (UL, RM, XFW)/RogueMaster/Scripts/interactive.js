let dialog = require("dialog");
let keyboard = require("keyboard");
let storage = require("storage");

// Need to run code from file, and filename must be unique
let tmp_template = "/ext/apps_data/js_app/.interactive.tmp.";
let tmp_number = 0;

let result = "Run JavaScript Code";
while (dialog.message("Interactive Console", result)) {
    keyboard.setHeader("Type JavaScript Code");
    let input = keyboard.text(256);
    if (!input) break;

    let path = tmp_template + to_string(tmp_number++);
    storage.write(path, "({run:function(){return " + input + ";},})");
    result = load(path).run();
    storage.remove(path);

    // Must convert to string explicitly
    if (typeof result === "number") {
        result = to_string(result);
    } else if (typeof result === "undefined") {
        result = "undefined";
    } else if (typeof result === "boolean") {
        result = result ? "true" : "false";
    } else if (typeof result === "object") {
        result = JSON.stringify(result);
    }
}
