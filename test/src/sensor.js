const chakram = require("chakram")
const expect = chakram.expect
const jsend = require("../jsend")
const consts = require("../consts")
const host = consts.host


describe("Sensor API", () => {

  if (consts.handlers.ht) {
    describe("GET /sensor/ht", () => {
      it("should return temperature and humidity", () => {
        const temperatureSchema = {
          type: "object",
          required: ["status", "data"],
          properties: {
            status: {
              enum: ["success"]
            },
            data: {
              properties: {
                temperature: {
                  required: true,
                  type: ["number"]
                },
                humidity: {
                  required: true,
                  type: ["number"]
                }
              }
            }
          }
        }
        const r = chakram.get(host + '/sensor/ht')
        return expect(r).to.have.schema(temperatureSchema)
      })
    })
  }

  if (consts.handlers.cds) {
    describe("GET /sensor/brightness", () => {
      it("should return brightness", () => {

        const brightnessSchema = {
          type: "object",
          required: ["status", "data"],
          properties: {
            status: {
              enum: ["success"]
            },
            data: {
              properties: {
                value: {
                  required: true,
                  type: ["number"]
                }
              }
            }
          }
        }
        const r = chakram.get(host + '/sensor/brightness')
        return expect(r).to.have.schema(brightnessSchema)
      })
    })
  }

})
