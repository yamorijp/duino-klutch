const path = require('path')
const docprint = require("docprint")


docprint({
  filepath: "apidoc/api.apib",
  destination: "data/docs/",
  header: "apidoc/header.html",
  css: "apidoc/style.css",
  title: "Duino Klutch API"
})

console.log("API document created.")
console.log("")
console.log("To upload contents to SPIFFS, exec this command.")
console.log("> pio run --target uploadfs")
